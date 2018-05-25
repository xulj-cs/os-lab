#include "x86.h"
#include "device.h"
#include "common.h"


#define RUNNABLE_QUEUE_MAX 10
static stQ_Node Runnable_queue[RUNNABLE_QUEUE_MAX];
static int tail,avil;

void init_runnable_queue(){
	stQ_init(Runnable_queue,RUNNABLE_QUEUE_MAX,&tail,&avil);
}

void runnable_queue_push_back(struct ProcessTable *p){
	int pcb_no = p - pcb;
	stQ_push(Runnable_queue,pcb_no,&tail,&avil);
}

struct ProcessTable* runnable_queue_pop_front(){
	
	int pcb_no;
	if(stQ_pop(Runnable_queue,&pcb_no,&tail,&avil)==FALSE)
		return pcb;
	else
		return pcb + pcb_no;
	
}

void schedule(){
	//从当前就绪进程队列中取队首，分配时间片

	current = runnable_queue_pop_front();
	current->state = STATE_RUNNING;
	if(current != IDLE)
		current->timeCount = TIMESLICE;

}
