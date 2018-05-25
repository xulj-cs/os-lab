#include "x86.h"
#include "device.h"

struct ProcessTable pcb[MAX_PCB_NUM];
struct ProcessTable *current;

void idle();

static void init_idle(){

//	pcb[0].tf.ss = KSEL(SEG_KDATA);
//	pcb[0].tf.esp = 0x140000;
  	
	pcb[0].tf.eflags = 0x216;
	pcb[0].tf.cs = KSEL(SEG_KCODE);

	void(*pFun)(void);
	pFun = idle;
	pcb[0].tf.eip = (uint32_t)(pFun);

	pcb[0].tf.gs = pcb[0].tf.fs = pcb[0].tf.es = pcb[0].tf.ds = KSEL(SEG_KDATA);
	pcb[0].state = STATE_RUNNABLE;


}

static void init_first_user_process(){
	
	pcb[1].tf.ss = USEL(SEG_UDATA);
	pcb[1].tf.esp = 0x800000;
	pcb[1].tf.eflags = 0x216; 
	pcb[1].tf.cs = USEL(SEG_UCODE);
	pcb[1].tf.eip = 0x200000;

	pcb[1].state = STATE_RUNNABLE;
	pcb[1].pid = 1;
}


void init_pcb(){
	memset(pcb,0,sizeof(pcb));
	init_idle();
	init_first_user_process();
}


int fork_pcb(){

	int i;
	for(i=1;i<MAX_PCB_NUM;i++){
		if(pcb[i].state==STATE_EMPTY)
			break;
	}
	assert(i<MAX_PCB_NUM);

	memset(&pcb[i],0,sizeof(struct ProcessTable));

	pcb[i].tf.ss = USEL(SEG_UDATA2);

	
	pcb[i].tf.esp = current->tf.esp ;

	pcb[i].tf.ebp = current->tf.ebp ;

	
	pcb[i].tf.eflags = current->tf.eflags;
	pcb[i].tf.cs =  USEL(SEG_UCODE2);
	pcb[i].tf.eip = current->tf.eip ;

	pcb[i].tf.eax = 0;//child progress ret value
	pcb[i].tf.ecx = current->tf.ecx;
	pcb[i].tf.edx = current->tf.edx;
	pcb[i].tf.ebx = current->tf.ebx;

	pcb[i].tf.esi = current->tf.esi;
	pcb[i].tf.edi = current->tf.edi;

	pcb[i].tf.ds = USEL(SEG_UDATA2);
	pcb[i].tf.es = USEL(SEG_UDATA2);
	pcb[i].tf.fs = USEL(SEG_UDATA2);
	pcb[i].tf.gs = USEL(SEG_UDATA2);

	pcb[i].sleepTime = 0;
	pcb[i].timeCount = 0;
	pcb[i].pid = i;
	pcb[i].state = STATE_RUNNABLE;
	runnable_queue_push_back(&pcb[i]);
	return i;
}

void exit_pcb(){

	current->state = STATE_EMPTY;
	
}
