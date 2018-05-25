#include "x86.h"
#include "device.h"
#define SYS_exit  1
#define SYS_fork  2
#define SYS_write 4
#define SYS_sleep 7

void schedule();

void syscallHandle(struct TrapFrame *tf);
void GProtectFaultHandle(struct TrapFrame *tf);
void timeIntrHandle();
 
void sys_write(struct TrapFrame *tf);
void sys_sleep(struct TrapFrame *tf);
void sys_fork(struct TrapFrame *tf);
void sys_exit(struct TrapFrame *tf);
//void asmDoIrq2();
//void schedule();

void irqHandle(struct TrapFrame *tf) {
	/*
	 * 中断处理程序
	 */
	/* Reassign segment register */
	
	asm volatile("cli");

	struct ProcessTable *old_current = current;

    asm volatile ("movw %%ax, %%ds" :: "a" (KSEL(SEG_KDATA)));
    asm volatile ("movw %%ax, %%es" :: "a" (KSEL(SEG_KDATA)));
    asm volatile ("movw %%ax, %%fs" :: "a" (KSEL(SEG_KDATA)));
    asm volatile ("movw %%ax, %%gs" :: "a" (KSEL(SEG_KDATA)));
	//Log("irq:%d\n",tf->irq);
	switch(tf->irq) {
		case -1:
			Log("Unhandled exception!\n");
			assert(0);
			break;
		case 0xd:
			GProtectFaultHandle(tf);
			break;
		case 0x80:
			syscallHandle(tf);
			break;
		case 1000:
			timeIntrHandle();
			break;
		case 1014:
		case 1001:
			break;
		default:
			Log("Unexpected exception #%d!\n",tf->irq);
			assert(0);
	}

	if(old_current != current){
		if(current == pcb){
			//tss.esp0 = (uint32_t )&current->tf.esp;
			//Log("user --> kernel\n");
			asm volatile("movl %0,%%eax" :: "g" (&current->tf));
		}
		else{
			tss.esp0 = (uint32_t)&current->state;
			//Log("kernel/user --> user\n");
			asm volatile("movl %0,%%eax" :: "g" (&current->tf));
		}
		///asmDoIrq2();
	}
	else{
		
			asm volatile("leal 12(%ebp),%eax");	
	}
	asm volatile("sti");
}

void syscallHandle(struct TrapFrame *tf) {
	/* 实现系统调用*/

	//Log("%d\n",tf->eax);
	switch(tf->eax){
		case SYS_write:sys_write(tf);break;
		case SYS_sleep:sys_sleep(tf);break;
		case SYS_fork:sys_fork(tf);break;
		case SYS_exit:sys_exit(tf);break;
		default:assert(0);break;
	}
}

void GProtectFaultHandle(struct TrapFrame *tf){
	assert(0);
	return;
}

void timeIntrHandle(){

    int i;
    for(i=0;i<MAX_PCB_NUM;i++){

        if(pcb[i].state == STATE_BLOCKED){
            pcb[i].sleepTime --;

            if(pcb[i].sleepTime==0){
                pcb[i].state = STATE_RUNNABLE;
				runnable_queue_push_back(&pcb[i]);
            }
        }

    }
	if(current != IDLE){
		current->timeCount --;
	    if(current->timeCount == 0)
		{	
			current->state = STATE_RUNNABLE;
			//当前进程时间片用完 将其加入就绪进程队尾
			runnable_queue_push_back(current);
		
		    schedule();
		}
	}
	else{
		current->state = STATE_RUNNABLE;
		schedule();			
	}
}

