#include "globals.h"

// Declarations
int pred(int x, int y, int x_prev, int y_prev, int size);
void* client_handler(void* args);
void init_map();
void init_game();
void init_player(int i);
void resolve_collisions(int* new_x,int* new_y);
void prepare_updates();
void close_game(pthread_t* thread_id);
void delete_bonus(int id);
void add_bonus();
void delete_invincibility(int id);
void velocity_bonus_expire(int id);
void size_bonus_expire(int id);
void player_collision_with_bonus(int player_number, int bonus_number);
void disconnect_player(int id);

int main(int argc, char* argv[]) {

	// Initialize game 
	init_game();
	// Initialize map
	init_map();
	// Server and client file descriptors
	int listenfd = 0, connfd = 0;
	// Structs holding info about connection on both sides
	struct sockaddr_in serv_addr, client_addr;
	socklen_t addr_size;
	char buffer[BUFFER_SIZE];
	// SOCK_STREAM - TCP/IP, AF_INET - IPv4
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	memset(&serv_addr, '0', sizeof(serv_addr));
	// Protocol family
	serv_addr.sin_family = AF_INET;
	// IP
	serv_addr.sin_addr.s_addr = inet_addr(IP);
	// Port
	serv_addr.sin_port = htons(PORT);
	// Bind server into specific port
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(listenfd, CLIENTS_AMOUNT);
	// Structure used to pass arguments to threads
	struct client_arg arg;
	// Seek connection
	printf("Listening on port number: %i\n", PORT);

	for(int i=0;i<CLIENTS_AMOUNT;i++) states[i].state.alive = -1;

	while(players_connected < CLIENTS_AMOUNT) {
		printf("Waiting for connection\n");
		connfd = accept(listenfd, (struct sockaddr*)&client_addr, &addr_size);
		printf("New connection recived %i/%i \n",players_connected+1,CLIENTS_AMOUNT);
		int idx;
		for(idx = 0; idx< CLIENTS_AMOUNT;idx++){
			if(states[idx].state.alive == -1) break;
		}
		printf("%i\n" ,idx);
		arg.connfd = connfd;
		arg.id = idx;
		pthread_create(&(thread_id[idx]), NULL, client_handler, (void*)&arg);
		init_player(idx);
	}
	// All clients connected and init array ready, allow them to run
	for (int i = 0; i < CLIENTS_AMOUNT; i++) {
		sem_post(&S0);
	}
	// Main loop
	while (running) {
		int new_x, new_y;
		if (bonuses_count < CLIENTS_AMOUNT && (((double)rand())/((double)RAND_MAX) <= BONUS_CHANCE)){
			add_bonus();
		}
		// Decrement semaphore by CLIENTS_AMOUNT
		for (int i = 0; i < CLIENTS_AMOUNT; i++) sem_wait(&S2);
		// Resolve collisions
		resolve_collisions(&new_x,&new_y);
		// Prepare updates
		prepare_updates();
		for (int i = 0; i < CLIENTS_AMOUNT; i++) sem_post(&(S3[i]));
		global_counter++;
		if(bq!= NULL && bq->k == global_counter)qexecute(&bq, global_counter);
	}
	// End of the game
	close_game(thread_id);
	return 0;
}

//
int pred(int x, int y, int x_prev, int y_prev, int size){
	return (x >= (x_prev - size)) && (x <= (x_prev + size)) && (y >= (y_prev - size)) && (y <= (y_prev + size));
}

// Each clients thread executes this in game loop
void* client_handler(void* args) {
	struct client_arg* arg = args;
	int sock = arg->connfd;
	int id = arg->id;
	printf("%i,%i\n",sock,id);
	// Message that will be sent to client upon connection
	char* message = "connected";
	// Buffer for recieved message
	char buffer[BUFFER_SIZE_SMALL];
	memset(buffer, 0, sizeof(buffer));
	// Struct with currently recieved player input
	struct player_input in;
	// Struct that will be sent every frame
	struct state_update update;
	// Boolean variables determining player state
	int left_pressed = 0,right_pressed = 0,angle = 0,alive = 1;
	// Packet of data that will be sent after initializing the game
	struct initialization_data packet;
	// Set timeout
	struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
	// Send connection message
	send(sock, message, strlen(message), 0);
	do{
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	    if(recv(sock,&buffer,BUFFER_SIZE_SMALL,0)<=0) {
			disconnect_player(id);
			pthread_exit(NULL);
		}
	    send(sock, &(struct lobby_data){players_connected, CLIENTS_AMOUNT}, sizeof(struct lobby_data), 0);
		//sleep(1);
		printf("%s\n",buffer);
	}while(players_connected != CLIENTS_AMOUNT);
	// Wait until all players connect
	sem_wait(&S0);
	timeout.tv_sec = 0;
    timeout.tv_usec = 333333;
	memcpy(packet.data, init_arr, CLIENTS_AMOUNT*sizeof(struct player_init_data));
	packet.session_ID = id;
	// Send init packet
	send(sock, &packet, sizeof(struct initialization_data), 0);
	// Assert that client recieved data
	recv(sock, &buffer, BUFFER_SIZE_SMALL, 0);
	if (strcmp(buffer, "connected")) {
		printf("error - recieved : %s", buffer);
	}
	else {
		// bool
		int connected = 1;
		int x;
		// Conenction succesful
		while (1) {
			setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
			fflush(stdout);
            if (connected) {
				x = recv(sock, &in, sizeof(struct player_input),0);
				if(x == 0){
					printf("ID %i disconnected\n",id);
					 connected = 0;
				}
				if(x > 0){
					// Calculate angle based on recieved input
					if (in.left == UP) left_pressed = 0;
					else if (in.left == DOWN) left_pressed = 1;
					if (in.right == UP) right_pressed = 0;
					else if (in.right == DOWN) right_pressed = 1;
					if (left_pressed) {
						angle -= ANGLE;
						if (angle < 0) angle += 360;
					}
					if (right_pressed) {
						angle += ANGLE;
						if (angle > 360) angle -= 360;
					}
				}
            }
            // If semaphore open  write to moves vector
            sem_wait(&S1);
			moves_vector[id].x = states[id].velocity * cos(angle * M_PI/180.0);
			moves_vector[id].y = states[id].velocity * sin(angle * M_PI/ 180.0);
			printf("(ID - %i) dx: %f\n",id,moves_vector[id].x);
			fflush(stdout);
			sem_post(&S1);
			sem_post(&S2);
			// If update packet is ready, send it
			sem_wait(&(S3[id]));
			if(connected && x>0){
			for (int i = 0; i < CLIENTS_AMOUNT; i++) {
				update.player_states[i] = states[i].state;
				update.bonuses[i] = bonuses[i];
			}
			update.running = running;
			send(sock, &update, sizeof(struct state_update), 0);
			}
			if(running == 0){
				printf("all dead\n");
				fflush(stdout);
				break;
			} 
		}
	}
	printf("umarlem za grzechy wasze  %i\n",id);
	fflush(stdout);
	sem_wait(&S4);
	//sleep(1);
	send(sock, &scoreboard,CLIENTS_AMOUNT*sizeof(int), 0);
	shutdown(sock,2);
	//sleep(1);
	pthread_exit(NULL);
}

// Initialize map info
void init_map(){
	for(int i = 0; i < MAX_PLAYERS; i++){
		start_xs[i] = (i+1)*(WIDTH/(MAX_PLAYERS+1));
		start_ys[i] = (i+1)*(HEIGHT/(MAX_PLAYERS+1));
	}	
	for(int i = 0 ; i < WIDTH; i++){
		collision_arr[i][0] = 1;
		collision_arr[i][HEIGHT-1] = 1;
	}
	for(int i=0;i<HEIGHT;i++){
		collision_arr[0][i] = 1;
		collision_arr[WIDTH-1][i] = 1;
	}
}

// Initialize game info
void init_game(){
	running = CLIENTS_AMOUNT;
	global_counter = 0;
	players_connected = 0;
	bonuses_count = 0;
	bq = NULL;
	srand(time(NULL));
	// Initialize semaphores
	if (sem_init(&S0, 0, 0) == -1) printf("\nerror\n");
	if (sem_init(&S1, 0, 1) == -1) printf("\nerror\n");
	if (sem_init(&S2, 0, 0) == -1) printf("\nerror\n");
	if (sem_init(&S4, 0, 0) == -1) printf("\nerror\n");
	for(int i=0;i<CLIENTS_AMOUNT;i++){
		if (sem_init(&(S3[i]), 0, 0) == -1) printf("\nerror\n");
	}
}

// Add init data for specific player to init array
void init_player(int i){
	init_arr[i].ID = i;
	for (int j = 0; j < 3; j++) {
		init_arr[i].color[j] = colors[i][j];
	}
	init_arr[i].start_x = start_xs[i];
	init_arr[i].start_y = start_ys[i];
	init_arr[i].size = DEFAULT_SIZE;
	positions[i].x = start_xs[i];
	positions[i].y = start_ys[i];
	states[i].state.alive = 1;
	states[i].state.size = DEFAULT_SIZE;
	states[i].velocity = DEFAULT_VELOCITY;
	states[i].state.bonus_modifier = 0;
	players_connected++;
}

// Resolve players collisions
void resolve_collisions(int* new_x,int* new_y){
	for (int i = 0; i < CLIENTS_AMOUNT; i++) {
			if(states[i].state.alive){
			*new_x = round(positions[i].x + moves_vector[i].x);
			*new_y = round(positions[i].y + moves_vector[i].y);
			printf("%i is on (%i,%i)\n",i,*new_x,*new_y);
			fflush(stdout);
			for (int x = *new_x - states[i].state.size; x <= *new_x + states[i].state.size; x++) {
				for (int y = *new_y - states[i].state.size; y <= *new_y + states[i].state.size; y++) {
					if(pred(x, y, round(positions[i].x),
					 round(positions[i].y), states[i].state.size) == 0){
						if (collision_arr[x][y] > 0){
							if (collision_arr[x][y] % 100 > 0){
								if(states[i].invincible != 1)
								{
								running--;
								printf("%i collided on (%i,%i)\n",i,*new_x,*new_y);
								fflush(stdout);
								scoreboard[running] = i;
								states[i].state.alive = 0;
								break;
								}
							}
							else player_collision_with_bonus(i,collision_arr[x][y]/100 - 1);
						}
					}
				}
				if (states[i].state.alive == 0) break;
				}	
		}
	}
	// Iterate over all clients, add their collisions to collision_arr
	for (int i = 0; i < CLIENTS_AMOUNT; i++) {
		*new_x = round(positions[i].x + moves_vector[i].x);
		*new_y = round(positions[i].y + moves_vector[i].y);

		for (int x = *new_x - states[i].state.size; x <= *new_x + states[i].state.size; x++) {
			for (int y = *new_y - states[i].state.size; y <= *new_y + states[i].state.size; y++) {
				if (states[i].state.draw && collision_arr[x][y] < 9) 
					collision_arr[x][y]++;

			}
		}
	}
}

// Prepare updated packages for players
void prepare_updates(){
	for (int i = 0; i < CLIENTS_AMOUNT; i++) {
			if (states[i].state.alive) {
				states[i].state.dx = moves_vector[i].x;
				states[i].state.dy = moves_vector[i].y;
				states[i].state.ID = i;
				if (global_counter % 200 > 180){
					states[i].state.draw = 0;
				}
					
				else{
					states[i].state.draw = 1;
				}
					
				positions[i].x += moves_vector[i].x;
				positions[i].y += moves_vector[i].y;
			}

		}
}

// Cleaning after game ends
void close_game(pthread_t* thread_id){
	for (int i = 0; i < CLIENTS_AMOUNT; i++) sem_post(&S4);
	for (int i = 0; i < CLIENTS_AMOUNT; i++) pthread_join(thread_id[i],NULL);
	// Close all semaphores
	sem_close(&S0);
	sem_close(&S1);
	sem_close(&S2);
	sem_close(&S4);
	for(int i=0;i<CLIENTS_AMOUNT;i++){
		sem_close(&(S3[i]));
	}
}

//
void delete_bonus(int id){
	bonuses[id].active = 0;

	for( int row = - BONUS_SIZE; row < BONUS_SIZE; row ++){
		for( int col = - BONUS_SIZE; col < BONUS_SIZE; col ++){
			if(bonuses[id].x + row >= 0 && bonuses[id].x + row < WIDTH && bonuses[id].y + col >=0 && bonuses[id].y + col < HEIGHT)
				collision_arr[bonuses[id].x + row][bonuses[id].y + col] -= 100*(id+1);
		}
	}
	bonuses_count--;
}

//
void add_bonus(){
	int i = 0;
	for(; i<CLIENTS_AMOUNT; ++i){
		if(bonuses[i].active == 0)
			break;
	}
	bonuses[i].active = 1;
	bonuses[i].x = rand() % WIDTH;
	bonuses[i].y = rand() % HEIGHT;
	bonuses[i].type = rand() % 3;
	for( int row = - BONUS_SIZE; row < BONUS_SIZE; row ++){
		for( int col = - BONUS_SIZE; col < BONUS_SIZE; col ++){
			if(bonuses[i].x + row >= 0 && bonuses[i].x + row < WIDTH && bonuses[i].y + col >=0 && bonuses[i].y + col < HEIGHT)
				collision_arr[bonuses[i].x + row][bonuses[i].y + col] += 100*(i+1);
		}
	}
	bonuses_count++;
	qinsert(&bq,delete_bonus, i,BONUS_TIME_EXPIRED, global_counter + BONUS_TIME);
}

//
void delete_invincibility(int id){ 
	states[id].invincible = 0;
}

//
void velocity_bonus_expire(int id){ states[id].velocity = DEFAULT_VELOCITY; }

//
void size_bonus_expire(int id){
	states[id].state.size=1;
	states[id].invincible = 1;
	qinsert(&bq,delete_invincibility, id ,PLAYER_BONUS_EXPIRED, global_counter + 3);
}

//
void player_collision_with_bonus(int player_number, int bonus_number){
	delete_bonus(bonus_number);
	if(bq != NULL)
		qremove(&bq, bonus_number, BONUS_TIME_EXPIRED);
		
	// Size
	if(bonuses[bonus_number].type == 0){
		states[player_number].state.size = 2;
		qinsert(&bq,size_bonus_expire, player_number ,PLAYER_BONUS_EXPIRED, global_counter + 100);
	}
	// Invincibility
	else if(bonuses[bonus_number].type == 1){
		states[player_number].invincible = 1;
		qinsert(&bq,delete_invincibility, player_number ,PLAYER_BONUS_EXPIRED, global_counter + 100);
	}
	// Velocity 
	else{
		states[player_number].velocity = 3;
		qinsert(&bq,velocity_bonus_expire, player_number ,PLAYER_BONUS_EXPIRED, global_counter + 100);
	}
}

//
void disconnect_player(int id) {
    states[id].state.alive = -1;
    players_connected--;
	thread_id[id] = 0;
}