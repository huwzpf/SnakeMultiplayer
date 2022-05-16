#ifndef LOGIC_H
#define LOGIC_H
#include "globals.h"
#include "bonus.h"

int pred(int x, int y, int x_prev, int y_prev, int size){
	return (x >= (x_prev - size)) && (x <= (x_prev + size)) && (y >= (y_prev - size)) && (y <= (y_prev + size));
}

// Resolve players collisions
void resolve_collisions(int* new_x,int* new_y){
	for (int i = 0; i < CLIENTS_AMOUNT; i++) {
			if(states[i].state.alive){
			*new_x = round(positions[i].x + moves_vector[i].x);
			*new_y = round(positions[i].y + moves_vector[i].y);
			for (int x = *new_x - states[i].state.size; x <= *new_x + states[i].state.size; x++) {
				for (int y = *new_y - states[i].state.size; y <= *new_y + states[i].state.size; y++) {
					if(pred(x, y, round(positions[i].x),
					 round(positions[i].y), states[i].state.size) == 0){
						if (collision_arr[x][y] > 0){
							if (collision_arr[x][y] % 100 > 0){
								if(states[i].invincible != 1)
								{
								running--;
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

#endif