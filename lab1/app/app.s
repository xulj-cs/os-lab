.code32

.global start
start:



        leal message,%esi
        movl $0,%ecx
        movl $((80*10+0)*2), %edi               #在第5行第0列打印
loop1:
        movb $0x0c, %ah                         #黑底红字
        movb (%esi),%al
	    movw %ax, %gs:(%edi)                    #写显存
        
		addl $2,%edi							#移动到下一位置
		inc %esi								#移动到下一字符
        inc %ecx								
        
		cmpl $13,%ecx
        jne loop1

loop2:
		jmp loop2
.p2align 2	
message:
   .string "Hello, World!"
