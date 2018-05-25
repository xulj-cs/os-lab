#include "types.h"
#include "boot.h"

#define SECTSIZE 512

void bootMain(void) {
	/* 加载内核至内存，并跳转执行 */
	void (*entry) (void);

//	uint8_t buf[SECTSIZE*200];

	struct ELFHeader* elfH = (void *)0x10000;
	readSeg((void *)elfH,SECTSIZE*200,0);
	
//	struct ELFHeader* elfH = (void *)buf;
	
//	const uint32_t elf_magic = 0x464c457f;
//	if(elfH->magic!=elf_magic)
//		while(1);
//	struct ProgramHeader *pH=(void*)&buf[elfH->phoff];

	struct ProgramHeader *pH=(void *)((uint32_t)elfH+elfH->phoff);
	int i;
	for(i=0; i<elfH->phnum; i++){
//		if(pH->type == PT_LOAD){
			readSeg((void *)pH->vaddr,pH->memsz,pH->off);
//		}
		pH++;
	}

	entry = (void(*)(void))(elfH->entry);
	entry();
}

//void waitDisk(void) { // waiting for disk
//	while((inByte(0x1F7) & 0xC0) != 0x40);
//}

void readSect(void *dst, int offset) { // reading a sector of disk
	int i;
//	waitDisk();
	while((inByte(0x1F7) & 0xC0) != 0x40);
	outByte(0x1F2, 1);
	outByte(0x1F3, offset);
	outByte(0x1F4, offset >> 8);
	outByte(0x1F5, offset >> 16);
	outByte(0x1F6, (offset >> 24) | 0xE0);
	outByte(0x1F7, 0x20);
	while((inByte(0x1F7) & 0xC0) != 0x40);
//	waitDisk();
	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = inLong(0x1F0);
	}
}

void readSeg(void *dst,uint32_t count,uint32_t offset){

	uint32_t beginAddr = (uint32_t)dst - offset%SECTSIZE;
	uint32_t endAddr = (uint32_t)dst + count;

	uint32_t beginSec = offset/SECTSIZE + 1;

	for(;beginAddr < endAddr;beginAddr += SECTSIZE){
		readSect((void *)beginAddr,beginSec);
		beginSec++;
	}	
}
