#include "x86.h"
#include "device.h"
#include <stdarg.h>
void initSerial(void) {
	outByte(SERIAL_PORT + 1, 0x00);
	outByte(SERIAL_PORT + 3, 0x80);
	outByte(SERIAL_PORT + 0, 0x01);
	outByte(SERIAL_PORT + 1, 0x00);
	outByte(SERIAL_PORT + 3, 0x03);
	outByte(SERIAL_PORT + 2, 0xC7);
	outByte(SERIAL_PORT + 4, 0x0B);
}

static inline int serialIdle(void) {
	return (inByte(SERIAL_PORT + 5) & 0x20) != 0;
}

void putChar(char ch) {
	while (serialIdle() != TRUE);
	outByte(SERIAL_PORT, ch);
}


static void putStr(const char *str){
    while(*str){
        putChar(*str);
        str++;
    }
}

static void putHex(unsigned n){
    if( n==0 ){
        putChar('0');
        return ;
    }
    if(n/16)
        putHex(n/16);
    char c ;
    if(n%16>9){
        c = n%16-10+'a';
    }
    else{
        c = n%16 +'0';
    }
    putChar(c);
}

static void putDec( int n){
    if(n==0){
        putChar('0');
        return;
    }
    if(n<0){
        if( n == 0x80000000){
            putStr("-2147483648");
            return ;
        }
        putChar('-');
        n = -n;
    }
    if(n/10)
        putDec(n/10);
    putChar(n%10+'0');
}

void Log(const char *format,...){
    va_list ap;
    va_start(ap,format);
    while(*format){

        if(*format != '%'){
            putChar(*format);
            format++;
        }
        else{
            format++;
            switch(*format){

                case 'c':{
                    const char c = va_arg(ap,int);
                    putChar(c);
                    format++;
                    break;
                }
                case 's':{
                    const char *str = va_arg(ap,char *);
                    putStr(str);
                    format++;
                    break;
                }
                case 'x':{
                    const int n = va_arg(ap,int);
                    putHex(n);
                    format++;
                    break;
                }
                case 'd':{
                    const int n = va_arg(ap,int);
                    putDec(n);
                    format++;
                }
            }
        }
    }

}

