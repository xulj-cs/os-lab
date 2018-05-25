#include "x86.h"
#include "device.h"
#include "common.h"

static struct ProcessTable *runnable_head;
static struct ProcessTable *runnable_tail;

void init_runnable_queue(){
	runnable_head = runnable_tail = NULL;
}

/* 
   three possible queues
	0.head == NULL && tail == NULL
	1.head != NULL && tail == NULL
	2.head != NULL && tail != NULL
*/
void runnable_queue_push_back(struct ProcessTable *p){
	//Log("push\n");
	//Log("%x::%x\n",(uint32_t)runnable_head,(uint32_t)runnable_tail);
	if(runnable_head==NULL )
		runnable_head = p;
	else if(runnable_tail==NULL)
		runnable_tail = p;
	else
		assert(0);
	//Log("%x::%x\n",(uint32_t)runnable_head,(uint32_t)runnable_tail);
}

struct ProcessTable* runnable_queue_pop_front(){
	//Log("pop\n");
	//Log("%x::%x\n",(uint32_t)runnable_head,(uint32_t)runnable_tail);
	if(runnable_head==NULL){
	
	//Log("%x::%x\n",(uint32_t)runnable_head,(uint32_t)runnable_head);
		return &pcb[0];
	}
	else if(runnable_tail==NULL){
		struct ProcessTable *t = runnable_head;
		runnable_head = NULL;
	//Log("%x::%x\n",(uint32_t)runnable_head,(uint32_t)runnable_head);
		return t;
	}
	else{
		struct ProcessTable *t = runnable_head;
		runnable_head = runnable_tail;
		runnable_tail = NULL;
	//Log("%x::%x\n",(uint32_t)runnable_head,(uint32_t)runnable_head);
		return t;
	}
	
}

void schedule(){
	//从当前就绪进程队列中取队首，分配时间片

	current = runnable_queue_pop_front();
	current->state = STATE_RUNNING;
	if(current != IDLE)
		current->timeCount = TIMESLICE;

}
