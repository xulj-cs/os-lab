#include "x86.h"
#include "device.h"
#include "common.h"

SegDesc gdt[NR_SEGMENTS];
TSS tss;

int video_row,video_col;

#define SECTSIZE 512
#define PT_LOAD 1

void init_pcb();
void init_sem();
void waitDisk(void) {
	while((inByte(0x1F7) & 0xC0) != 0x40); 
}

void readSect(void *dst, int offset) {
	int i;
	waitDisk();
	
	outByte(0x1F2, 1);
	outByte(0x1F3, offset);
	outByte(0x1F4, offset >> 8);
	outByte(0x1F5, offset >> 16);
	outByte(0x1F6, (offset >> 24) | 0xE0);
	outByte(0x1F7, 0x20);

	waitDisk();
	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = inLong(0x1F0);
	}
}

void readSeg(void *dst,uint32_t count,uint32_t offset){

    uint32_t beginAddr = (uint32_t)dst - offset%SECTSIZE;
    uint32_t endAddr = (uint32_t)dst + count;

    uint32_t beginSec = offset/SECTSIZE + 201;

    for(;beginAddr < endAddr;beginAddr += SECTSIZE){
        readSect((void *)beginAddr,beginSec);
        beginSec++;
    }
}

void initSeg() {
	gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_KERN);
	gdt[SEG_KDATA] = SEG(STA_W,         0,       0xffffffff, DPL_KERN);
	gdt[SEG_UCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_USER);
	gdt[SEG_UDATA] = SEG(STA_W,         0,       0xffffffff, DPL_USER);
	gdt[SEG_UCODE2] = SEG(STA_X | STA_R, 0x2000,       0xffffffff, DPL_USER);
	gdt[SEG_UDATA2] = SEG(STA_W,         0x2000,       0xffffffff, DPL_USER);
	gdt[SEG_TSS] = SEG16(STS_T32A,      &tss, sizeof(TSS)-1, DPL_KERN);
	gdt[SEG_TSS].s = 0;
	gdt[SEG_KVIDEO] = SEG(STA_W,  0xb8000,       0xffffffff, DPL_KERN);
	setGdt(gdt, sizeof(gdt));

	/*
	 * 初始化TSS
	 */
	tss.ss0 = KSEL(SEG_KDATA);
//	tss.esp0 = 0x140000;			// kernel stack
	asm volatile("ltr %%ax":: "a" (KSEL(SEG_TSS)));

	/*设置正确的段寄存器*/
    asm volatile ("movw %%ax, %%gs" :: "a" (KSEL(SEG_KVIDEO)));
    asm volatile ("movw %%ax, %%fs" :: "a" (USEL(SEG_UDATA)));
    asm volatile ("movw %%ax, %%es" :: "a" (KSEL(SEG_KDATA)));
    asm volatile ("movw %%ax, %%ds" :: "a" (KSEL(SEG_KDATA)));
    asm volatile ("movw %%ax, %%ss" :: "a" (KSEL(SEG_KDATA)));

	lLdt(0);
	
}

void enterUserSpace(uint32_t entry) {
	/*
	 * Before enter user space 
	 * you should set the right segment registers here
	 * and use 'iret' to jump to ring3
	 */
	

    asm volatile ("movw %%ax, %%ds" :: "a" (USEL(SEG_UDATA)));
    asm volatile ("movw %%ax, %%es" :: "a" (USEL(SEG_UDATA)));
//	asm volatile ("movl $0x140000,%esp");//kernel stack esp
										// cs: kernel_code
										// ss: kernel_data
/*
    asm volatile ("movl %%eax, %%edx" :: "a" (USEL(SEG_UDATA)));
	asm volatile ("pushl %edx");//user stack ss
    asm volatile ("movl $0x800000,%eax");
	asm volatile ("pushl %eax");//user stack esp
	asm volatile ("sti");//enable interupt
	asm volatile ("pushfl"); //eflags
    asm volatile ("movl %%eax, %%edx" :: "a" (USEL(SEG_UCODE)));
	asm volatile ("pushl %edx"); //user code cs
    asm volatile ("movl %%eax, %%ebx" :: "a" (entry));
	asm volatile ("pushl %ebx");//user code entry: 0x200000
	asm volatile ("iret");//kernel -> user space
	
*/

	//p1 kernel stack 
//	tss.ss0 = KSEL(SEG_KDATA);
//	asm volatile("ltr %%ax":: "a" (KSEL(SEG_TSS)));

	current = pcb + 1;
	current->state = STATE_RUNNING;
	current->timeCount = TIMESLICE;

	tss.esp0 = (uint32_t)&current->state;			
    asm volatile ("movl %%eax, %%esp" :: "a" (&current->tf.eip));
	asm volatile ("iret");//kernel -> user space p1 
}

void loadUMain(void) {

	/*加载用户程序至内存*/


    struct ELFHeader* elfH = (void *)0x10000;
    readSeg((void *)elfH,SECTSIZE*2,0);


	const uint32_t elf_magic = 0x464c457f;
	assert(elfH->magic==elf_magic);

    struct ProgramHeader *pH=(void *)((uint32_t)elfH+elfH->phoff);
    int i;
    for(i=0; i<elfH->phnum; i++){
        if(pH->type == PT_LOAD){
            readSeg((void *)pH->vaddr,pH->filesz,pH->off);
			memset((void *)(pH->vaddr+pH->filesz),0,pH->memsz-pH->filesz);
        }
        pH++;
    }
	video_row = 5;
	video_col = 0;
	init_pcb();
	init_sem();
	init_runnable_queue();
	enterUserSpace(elfH->entry);

}


