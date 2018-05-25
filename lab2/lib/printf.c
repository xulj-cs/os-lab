#include "types.h"
#include <stdarg.h>

#define BUFSIZE 1024
#define SYS_write 4

int32_t syscall(int num, uint32_t a1,uint32_t a2,
		uint32_t a3,uint32_t a4,uint32_t a5);

static char buffer[BUFSIZE];
static int length ;
 
static void printChar( char c){
    buffer[length++] = c;
	if(length==BUFSIZE|| c =='\n' || c == '\r' ){
		syscall(SYS_write,1,(uint32_t)buffer,length,0,0);
		length = 0;
	}
}

static void printStr(const char *str){
    while(*str){
        printChar(*str);
        str++;
    }
}

static void printHex(unsigned n){
    if( n==0 ){
        printChar('0');
        return ;
    }
    if(n/16)
        printHex(n/16);
    char c ;
    if(n%16>9){
        c = n%16-10+'a';
    }
    else{
        c = n%16 +'0';
    }
    printChar(c);
}

static void printDec( int n){
    if(n==0){
        printChar('0');
        return;
    }
    if(n<0){
        if( n == 0x80000000){
            printStr("-2147483648");
            return ;
        }
        printChar('-');
        n = -n;
    }
    if(n/10)
        printDec(n/10);
    printChar(n%10+'0');
}

void printf(const char *format,...){
    va_list ap;
    va_start(ap,format);
	length = 0;
    while(*format){

        if(*format != '%'){
            printChar(*format);
            format++;
        }
        else{
            format++;
            switch(*format){

                case 'c':{
                    const char c = va_arg(ap,int);
                    printChar(c);
                    format++;
                    break;
                }
                case 's':{
                    const char *str = va_arg(ap,char *);
                    printStr(str);
                    format++;
                    break;
                }
                case 'x':{
                    const int n = va_arg(ap,int);
                    printHex(n);
                    format++;
                    break;
                }
                case 'd':{
                    const int n = va_arg(ap,int);
                    printDec(n);
                    format++;
                }
            }
        }
    }
	
	if(length){
		syscall(SYS_write,1,(uint32_t)buffer,length,0,0);
	}
}
