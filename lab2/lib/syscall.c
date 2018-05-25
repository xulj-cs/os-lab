#include "lib.h"
#include "types.h"
int32_t syscall(int num, uint32_t a1,uint32_t a2,
		uint32_t a3,uint32_t a4,uint32_t a5)
{
	int32_t ret = 0;

	/* 内嵌汇编 保存 num, a1, a2, a3, a4, a5 至通用寄存器*/
	asm volatile("int $0x80": "=a"(ret) : "a"(num) , "b"(a1) , "c"(a2) , "d"(a3 ) ,"S"(a4),"D"(a5)) ;
		
	return ret;
}

