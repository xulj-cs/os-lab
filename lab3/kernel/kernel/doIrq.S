.code32

.global irqEmpty
irqEmpty:
	pushl $0 // push dummy error code
	pushl $-1 // push interrupt vector into kernel
	jmp asmDoIrq

.global irqDivideError
irqDivideError:
	pushl $0
	pushl $0
	jmp asmDoIrq

.global irqDebugExcept
irqDebugExcept:
	pushl $0
	pushl $1
	jmp asmDoIrq

.global irqBreakpoint
irqBreakpoint:
	pushl $0
	pushl $3
	jmp asmDoIrq

.global irqOverflow
irqOverflow:
	pushl $0
	pushl $4
	jmp asmDoIrq

.global irqBoundsCheck
irqBoundsCheck:
	pushl $0
	pushl $5
	jmp asmDoIrq

.global irqInvalidOpcode
irqInvalidOpcode:
	pushl $0
	pushl $6
	jmp asmDoIrq

.global irqCopNotAvail
irqCopNotAvail:
	pushl $0
	pushl $7
	jmp asmDoIrq

.global irqDoubleFault
irqDoubleFault:
	pushl $8
	jmp asmDoIrq

.global irqCopSegment
irqCopSegment:
	pushl $0
	pushl $9
	jmp asmDoIrq

.global irqInvalidTSS
irqInvalidTSS:
	pushl $10
	jmp asmDoIrq

.global irqSegmentNotPre
irqSegmentNotPre:
	pushl $11
	jmp asmDoIrq

.global irqStackExcept
irqStackExcept:
	pushl $12
	jmp asmDoIrq

.global irqGProtectFault
irqGProtectFault:
	pushl $13
	jmp asmDoIrq

.global irqPageFault
irqPageFault:
	pushl $14
	jmp asmDoIrq

.global irqCopError
irqCopError:
	pushl $16
	jmp asmDoIrq

.global irqSyscall
irqSyscall:
	pushl $0 // push dummy error code
	pushl $0x80 // push interrupt vector into kernel stack
	jmp asmDoIrq

.global irq0
irq0:
	pushl $0
	pushl $1000
	jmp asmDoIrq

.global irq1
irq1:
	pushl $0
	pushl $1001
	jmp asmDoIrq

.global irq14
irq14:
	pushl $0
	pushl $1014
	jmp asmDoIrq

.global asmDoIrq
asmDoIrq:
	pushal // push process state into kernel stack
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs
	pushl %esp
	call irqHandle
#	addl $4, %esp
	movl %eax,%esp
	popl %gs
	popl %fs
	popl %es
	popl %ds
	popal
	addl $4, %esp //interrupt vector is on top of kernel stack
	addl $4, %esp //error code is on top of kernel stack
	iret
