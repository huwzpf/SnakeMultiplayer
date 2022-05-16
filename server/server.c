#include "init.h"
#include "bonus.h"
#include "logic.h"
#include "connection.h"

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

