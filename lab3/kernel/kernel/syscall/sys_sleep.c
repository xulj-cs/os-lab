#include "device.h"
#include "x86.h"

void schedule();

static int sleep(uint32_t sec){
	current->sleepTime = sec;
	current->state = STATE_BLOCKED;
	schedule();
	return 0;
}
void sys_sleep(struct TrapFrame *tf){
	
//	asm volatile ("movw %%ax, %%gs" :: "a" (KSEL(SEG_KVIDEO)));	
	tf->eax = sleep(tf->ebx);
}
