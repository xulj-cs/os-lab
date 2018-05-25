#include  "x86.h"
int exit_pcb();
void schedule();


static void exit(){
	exit_pcb();	 
	schedule();
}

void sys_exit(struct TrapFrame *tf){
	exit();
}
