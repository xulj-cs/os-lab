#include "device.h"

static inline void waitForInterrupt() {
    asm volatile("hlt");
}

void idle(){
	asm volatile("addl $0x4,%esp");
	while(1){
//		Log("wait...\n");
    	waitForInterrupt();
	}

}
