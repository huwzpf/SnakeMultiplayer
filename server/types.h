#ifndef PACKETS_H
#define PACKETS_H
#include "config.h"

struct bonus {
	int x;
	int y;
	int type;
	int active;
};

typedef enum key_state {
	UP,
	NO_CHANGE,
	DOWN
}key_state;

struct __attribute__((__packed__))  player_input {
	key_state left;
	key_state right;
};

struct __attribute__((__packed__))  lobby_data {
	int current;
	int target;
};

struct __attribute__((__packed__)) vector {
	double x;
	double y;
};

struct __attribute__((__packed__)) player_state {
	int ID;
	int alive;
	int draw;
	double dx;
	double dy;
	double size;
	int bonus_modifier;
};

struct __attribute__((__packed__)) player_state_2 {
	struct player_state state;
	double velocity;
	int invincible;
};

struct __attribute__((__packed__)) state_update {
	struct player_state player_states[CLIENTS_AMOUNT];
	struct bonus bonuses[CLIENTS_AMOUNT];
	int running;
};

struct __attribute__((__packed__)) player_init_data {
	int ID;
	int color[RGB];
	double start_x;
	double start_y;
	double size;
};

struct __attribute__((__packed__)) initialization_data {
	struct player_init_data data[CLIENTS_AMOUNT];
	int session_ID;
};

struct  __attribute__((__packed__)) client_arg{
	int connfd;
	int id;
};
	
#endif
