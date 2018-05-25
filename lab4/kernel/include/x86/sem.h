#ifndef _SEM_H_
#define _SEM_H_

#include "common/stQ.h"

#define SEM_QUEUE_MAX 10


struct Semaphore {
	int value;
	//int queue;
	stQ_Node queue[SEM_QUEUE_MAX];
	int queue_tail;
	int queue_avil;

	int used;
};

typedef struct Semaphore Semaphore;
typedef unsigned int sem_t;

#endif
