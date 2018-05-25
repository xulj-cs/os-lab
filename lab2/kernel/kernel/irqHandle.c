#include "x86.h"
#include "device.h"
#define SYS_write 4
void syscallHandle(struct TrapFrame *tf);

void GProtectFaultHandle(struct TrapFrame *tf);

void sys_write(struct TrapFrame *tf);

void irqHandle(struct TrapFrame *tf) {
	/*
	 * 中断处理程序
	 */
	/* Reassign segment register */
	
    asm volatile ("movw %%ax, %%gs" :: "a" (KSEL(SEG_KVIDEO)));
    asm volatile ("movw %%ax, %%ds" :: "a" (KSEL(SEG_KDATA)));
    asm volatile ("movw %%ax, %%es" :: "a" (KSEL(SEG_KDATA)));
    asm volatile ("movw %%ax, %%fs" :: "a" (KSEL(SEG_KDATA)));

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
		case 1001:
		case 1014:
			break;
		default:
			Log("Unexpected exception #%d!\n",tf->irq);
			assert(0);
	}
    asm volatile ("movw %%ax, %%ds" :: "a" (USEL(SEG_UDATA)));
    asm volatile ("movw %%ax, %%es" :: "a" (USEL(SEG_UDATA)));
    asm volatile ("movw %%ax, %%fs" :: "a" (USEL(SEG_UDATA)));
    asm volatile ("movw %%ax, %%gs" :: "a" (USEL(SEG_UDATA)));
}

void syscallHandle(struct TrapFrame *tf) {
	/* 实现系统调用*/

	switch(tf->eax){
		case SYS_write:sys_write(tf);break;
		default:assert(0);break;
	}
}

void GProtectFaultHandle(struct TrapFrame *tf){
	assert(0);
	return;
}
