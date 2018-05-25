#include "x86.h"
#include "device.h"

#define SEM_MAX 10
static Semaphore semList[SEM_MAX];

void schedule();

void init_sem(){
	//memset(semList,0,sizeof(Semaphore)*SEM_MAX);
	int i;
	for(i=0;i<SEM_MAX;i++){
		semList[i].used = 0;
	}
}


static void wakeup(Semaphore *sem){
	int pcb_no;
	stQ_pop(sem->queue,&pcb_no,&sem->queue_tail,&sem->queue_avil);
	pcb[pcb_no].state = STATE_RUNNABLE;
	runnable_queue_push_back(&pcb[pcb_no]);
}

static void asleep(Semaphore *sem){
	int pcb_no = current-pcb;
	stQ_push(sem->queue,pcb_no,&sem->queue_tail,&sem->queue_avil);
	current->sleepTime = -1;
	current->state = STATE_BLOCKED;
	schedule();
}

static int sem_init(sem_t *s,uint32_t value){
	int i;
	for(i=0;i<SEM_MAX;i++){
		if(semList[i].used == 0){
			semList[i].used = 2;
			semList[i].value = value;
			stQ_init(semList[i].queue,SEM_QUEUE_MAX,&semList[i].queue_tail,&semList[i].queue_avil);
			*s = (uint32_t)&semList[i];
			return 0;
		}
	}	
	assert(0);
	return -1;
}
static int sem_post(sem_t *s){
	Semaphore* sem = (void *)*s;
	if(sem < semList || sem> semList+SEM_MAX-1 || sem->used == 0){
		assert(0);
		return -1;
	}
	sem->value++;
	if(sem->value<=0){
		wakeup(sem);
	}

	return 0;
}

static int sem_wait(sem_t *s){
	Semaphore* sem = (void *)*s;
	if(sem < semList || sem> semList+SEM_MAX-1 || sem->used == 0){
		assert(0);
		return -1;
	}
	sem->value--;
	if(sem->value<0){
		asleep(sem);
	}
	return 0;
}

static int sem_destroy(sem_t *s){
	Semaphore* sem =(void *) *s;
	if(sem < semList || sem> semList+SEM_MAX-1 || sem->used == 0)
		return -1;
	sem->used --;
	return 0;
}

void sys_seminit(struct TrapFrame *tf){
	tf->eax = sem_init((void *)tf->ebx,tf->ecx);
}

void sys_sempost(struct TrapFrame *tf){
	tf->eax = sem_post((void *)tf->ebx);
}

void sys_semwait(struct TrapFrame *tf){
	tf->eax = sem_wait((void *)tf->ebx);
}

void sys_semdestroy(struct TrapFrame *tf){
	tf->eax = sem_destroy((void *)tf->ebx);
}

