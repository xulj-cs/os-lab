#include "types.h"
#define SYS_exit 1

int32_t syscall(int num, uint32_t a1,uint32_t a2,
		uint32_t a3,uint32_t a4,uint32_t a5);

void exit(){
	syscall(SYS_exit,0,0,0,0,0);
}
