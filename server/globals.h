#ifndef GLOBALS_H
#define GLOBALS_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <math.h>
#include "queue.h"
#include "types.h"

// Global variables
int running;
int global_counter;
int scoreboard[CLIENTS_AMOUNT];
int players_connected;
int bonuses_count;
int collision_arr[WIDTH][HEIGHT] = {0};
struct vector positions[CLIENTS_AMOUNT];
sem_t S0, S1, S2, S3[CLIENTS_AMOUNT], S4;
pqueue* bq;
pthread_t thread_id[CLIENTS_AMOUNT];
// PLayers colors
int colors[MAX_PLAYERS][RGB] = { {255, 0, 0},{0, 255, 0},{0, 0, 255},{255, 255, 0},
{255,0,255},{0,255,255} };
// Players starting positions
int start_xs[MAX_PLAYERS];
int start_ys[MAX_PLAYERS];
// Packages
struct bonus bonuses[CLIENTS_AMOUNT];
struct vector moves_vector[CLIENTS_AMOUNT];
struct player_state_2 states[CLIENTS_AMOUNT];
struct player_init_data init_arr[CLIENTS_AMOUNT];

#endif