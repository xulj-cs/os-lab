#ifndef __X86_PCB_H__
#define __X86_PCB_H__

#define MAX_STACK_SIZE (1 << 12)
#define MAX_PCB_NUM 5

#define STATE_EMPTY    0
#define STATE_RUNNABLE 1
#define STATE_RUNNING  2
#define STATE_BLOCKED  3

#define IDLE pcb
#define TIMESLICE 10 //100ms


struct ProcessTable {
    uint32_t stack[MAX_STACK_SIZE]; // 内核堆栈
    struct TrapFrame tf;
    int state;
    int timeCount;
    int sleepTime;
    uint32_t pid;
};
extern struct ProcessTable pcb[MAX_PCB_NUM];
extern struct ProcessTable *current;

void init_runnable_queue();
void runnable_queue_push_back(struct ProcessTable *);
struct ProcessTable * runnable_queue_pop_front();

#endif
