#ifndef QUEUE_H
#define QUEUE_H

#include"stdio.h"
#include"stdlib.h"

typedef enum event_type {PLAYER_BONUS_EXPIRED, BONUS_TIME_EXPIRED} event_type;

typedef struct pqueue pqueue;
struct pqueue {
	int k;
	void(*func)(int);
	int arg;
	event_type type;
	pqueue* next;
	pqueue* prev;
};

void qinsert(pqueue** phead, void(*func)(int), int arg, event_type type, int k);
void qexecute(pqueue** phead, int k);
void qremove(pqueue** phead, int arg, event_type type);

#endif