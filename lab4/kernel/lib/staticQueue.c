#include "common.h"
#include "x86.h"

void stQ_init(stQ_Node stQ[],int n,int *tail,int *avil){
	stQ[0].next = -1;
	int i;
	for(i=1;i<n-1;i++){
		stQ[i].next = i+1;
	}
	stQ[n-1].next = -1;
	*tail = 0;
	*avil = 1;
}
boolean stQ_push(stQ_Node stQ[],int data,int *tail,int *avil){
	if(*avil==-1){
		return FALSE; 
	}
	stQ[*tail].next = *avil;
	*tail = *avil;
	stQ[*tail].data = data;
	*avil = stQ[*avil].next;
	stQ[*tail].next = -1;

	return TRUE;
		
}

boolean stQ_pop(stQ_Node stQ[],int *data,int *tail,int *avil){

	int realHead = stQ[0].next;
	if(realHead==-1)
		return FALSE;
	if(*tail == realHead)
		*tail = 0;

	int next = stQ[realHead].next;
	stQ[0].next = next;
	
	stQ[realHead].next = *avil;
	*avil = realHead;
	
	*data =  stQ[realHead].data;
	return TRUE;
}
