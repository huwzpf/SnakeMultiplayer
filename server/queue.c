#include "queue.h"

void qinsert(pqueue** phead, void(*func)(int), int arg, event_type type, int k) {
	
	if (*phead == NULL) {
		*phead = (pqueue*)malloc(sizeof(pqueue));
		if (*phead != NULL) {
			(*phead)->k = k;
			(*phead)->arg = arg;
			(*phead)->func = func;
			(*phead)->type = type;
			(*phead)->next = NULL;
			(*phead)->prev = NULL;
		}
		else {
			printf("ERROR");
		}
	}
	else {
		pqueue* t = *phead;
		pqueue* new_elem = (pqueue*)malloc(sizeof(pqueue));
		while (t->k < k && t->next != NULL)
			t = t->next;
		
		new_elem->k = k;
		new_elem->arg = arg;
		new_elem->func = func;
		new_elem->type = type;
		
		if (t == *phead && k < t->k) {
			new_elem->next = t;
			t->prev = new_elem;
			new_elem->prev = NULL;
			*phead = new_elem;
		}
		else {
			new_elem->prev = t;
			new_elem->next = t->next;
			if (t->next != NULL)
				t->next->prev = new_elem;
			t->next = new_elem;
		}

	}
}

void qexecute(pqueue** phead, int k) {
	pqueue* t = *phead;
	while (t!=NULL && t->k == k) {
		*phead = t->next;
		if (t->next != NULL)
			t->next->prev = t->prev;
		t->func(t->arg);
		free(t);
		t = *phead;
	}

}

void qremove(pqueue** phead, int arg, event_type type) {
	pqueue* t = *phead;
	while (t->type != type && t->arg != arg) {
		if (t->next == NULL) {
			break;
		}
		t = t->next;
	}
	if(t->type != type || t->arg != arg)
		return;

	if (t == *phead) {
		*phead = t->next;
	}
	else {
		t->prev->next = t->next;
	}
	if (t->next != NULL)
		t->next->prev = t->prev;
	free(t);
	
}
