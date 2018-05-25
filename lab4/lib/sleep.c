#include "types.h"
#define SYS_sleep 7

int32_t syscall(int num, uint32_t a1,uint32_t a2,
		uint32_t a3,uint32_t a4,uint32_t a5);

void sleep(unsigned int sec){
	syscall(SYS_sleep,sec,0,0,0,0);
}
