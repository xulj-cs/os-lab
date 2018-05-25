#include "common.h"
#include "x86.h"
#include "device.h"

void kEntry(void) {

	initSerial();// initialize serial port
	initIdt(); // initialize idt
	initIntr(); // initalize 8259a
	initTimer();//initalize timer
	initSeg(); // initialize gdt, tss
	loadUMain(); // load user program, enter user space

	while(1);
	assert(0);
}
