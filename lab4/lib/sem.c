#include "types.h"
#define SYS_seminit 8
#define SYS_sempost 9
#define SYS_semwait 10
#define SYS_semdestroy 11

int32_t syscall(int num,uint32_t a1,uint32_t a2,
		uint32_t a3,uint32_t a4,uint32_t a5);

int sem_init(sem_t *sem,uint32_t value){
	return syscall(SYS_seminit,(uint32_t)sem,value,0,0,0);
}


int sem_post(sem_t *sem){
	return syscall(SYS_sempost,(uint32_t)sem,0,0,0,0);
}

int sem_wait(sem_t *sem){
	return syscall(SYS_semwait,(uint32_t)sem,0,0,0,0);
}

int sem_destroy(sem_t *sem){
	return syscall(SYS_semdestroy,(uint32_t)sem,0,0,0,0);
}
