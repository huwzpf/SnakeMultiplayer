#ifndef INIT_H
#define INIT_H

#include "globals.h"

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

#endif