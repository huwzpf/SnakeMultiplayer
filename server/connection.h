#ifndef CONNECTION_H
#define CONNECTION_H
#include "globals.h"

// Prepare updated packages for players
void prepare_updates(){
	for (int i = 0; i < CLIENTS_AMOUNT; i++) {
			if (states[i].state.alive) {
				states[i].state.dx = moves_vector[i].x;
				states[i].state.dy = moves_vector[i].y;
				states[i].state.ID = i;
				if (global_counter % GAP_FREQ > GAP_FREQ - 20) states[i].state.draw = 0;
				else states[i].state.draw = 1;
				positions[i].x += moves_vector[i].x;
				positions[i].y += moves_vector[i].y;
			}
		}
}

// Cleaning after game ends
void close_game(pthread_t* thread_id){
	for (int i = 0; i < CLIENTS_AMOUNT; i++) sem_post(&S4);
    sleep(1);
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

void disconnect_player(int id) {
    states[id].state.alive = -1;
    players_connected--;
	thread_id[id] = 0;
}

// Each clients thread executes this in game loop
void* client_handler(void* args) {
	struct client_arg* arg = args;
	int sock = arg->connfd;
	int id = arg->id;
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
	}while(players_connected != CLIENTS_AMOUNT);
	// Wait until all players connect
	sem_wait(&S0);
	timeout.tv_sec = 0;
    timeout.tv_usec = 33333;
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
		int connected = 1; // is player connected
		int x; // recv msg code
		// Conenction succesful
		while (1) {
			//setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
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
			if(running == 0)break;
		}
	}
	sem_wait(&S4);
	sleep(1);
	send(sock, &scoreboard,CLIENTS_AMOUNT*sizeof(int), 0);
	shutdown(sock,2);
	pthread_exit(NULL);
}

#endif