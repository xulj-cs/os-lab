
void printChar(char c,int row,int col){

	int position = (row*80+col)*2;
	asm volatile("movb $0x0c,%%bh\n\t"
				 "movb %0,%%bl\n\t"
				 "movl %1,%%edi\n\t"
				 "movw %%bx,%%gs:(%%edi)"
	   		 	 :
	 			 :"g"(c),"g"(position)
				 :"%ebx","%edi");

}
