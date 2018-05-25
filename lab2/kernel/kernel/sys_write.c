#include "device.h"
#include "x86.h"

static int write(int fd,void *buf,int len){
	if(fd == 1){
		int i;
		char c;
		for(i=0;i<len;i++){
			c = *((char *)buf+i);
			if(c=='\n'){
				video_row++;
				video_col=0;
			}
			else{
				printChar(c,video_row,video_col);
				video_col++;
				if(video_col == 80){
					video_col=0;
					video_row++;
				}
			}
		}

		return len;
	}
	return -1;
}

void sys_write(struct TrapFrame *tf){
	
//	asm volatile ("movw %%ax, %%gs" :: "a" (KSEL(SEG_KVIDEO)));	
	tf->eax = write(tf->ebx,(void *)tf->ecx,tf->edx);
}
