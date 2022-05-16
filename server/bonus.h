#ifndef BONUS_H
#define BONUS_H
#include "globals.h"

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

void delete_invincibility(int id){ 
    states[id].invincible = 0;
    states[id].state.bonus_modifier = 0;
}

void velocity_bonus_expire(int id){ 
    states[id].velocity = DEFAULT_VELOCITY;
    states[id].invincible = 1;
    states[id].state.bonus_modifier = 2;
    qinsert(&bq,delete_invincibility, id ,PLAYER_BONUS_EXPIRED, global_counter + 12 );
 }

void size_bonus_expire(int id){
	states[id].state.size = DEFAULT_SIZE;
	states[id].invincible = 1;
    states[id].state.bonus_modifier = 2;
	qinsert(&bq,delete_invincibility, id ,PLAYER_BONUS_EXPIRED, global_counter + 12 );
}

void player_collision_with_bonus(int player_number, int bonus_number){
	delete_bonus(bonus_number);
	if(bq != NULL) qremove(&bq, bonus_number, BONUS_TIME_EXPIRED);
	// Size
	if(bonuses[bonus_number].type == 0){
		states[player_number].state.size = DEFAULT_SIZE*2;
        states[player_number].state.bonus_modifier = 1;
		qinsert(&bq,size_bonus_expire, player_number ,PLAYER_BONUS_EXPIRED, global_counter + 100);
	}
	// Invincibility
	else if(bonuses[bonus_number].type == 1){
		states[player_number].invincible = 1;
        states[player_number].state.bonus_modifier = 2;
		qinsert(&bq,delete_invincibility, player_number ,PLAYER_BONUS_EXPIRED, global_counter + 100);
	}
	// Velocity 
	else{
		states[player_number].velocity = DEFAULT_VELOCITY*2;
        states[player_number].state.bonus_modifier = 3;
		qinsert(&bq,velocity_bonus_expire, player_number ,PLAYER_BONUS_EXPIRED, global_counter + 100);
	}
}

#endif