#include "common.h"
#include  "x86.h"
#include "device.h"
int fork_pcb();


static void fork_mem(){

	uint32_t src  = 0x200000;
	uint32_t size = 0x002000;
	uint32_t dst = src + size;
	memcpy((void *)dst,(void *)src,size);
}

static void fork_user_stack(){
	
	//[cuurent->esp,0x800000] -> [esp,0x802000]

	uint32_t src = current->tf.esp;
	int size = 0x800000 - src;
	uint32_t dst = 0x802000 - size;

	memcpy((void *)dst,(void *)src,size);

}
static int fork(){
	fork_mem();
	fork_user_stack();
	return fork_pcb();	 
}

void sys_fork(struct TrapFrame *tf){
	tf->eax = fork();//father progress ret value
}
