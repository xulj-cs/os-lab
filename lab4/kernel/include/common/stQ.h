#ifndef _STQ_H_
#define _STQ_H_


typedef struct Node{
	int data;
	int next;
}stQ_Node;

void stQ_init(stQ_Node stQ[],int n,int *tail,int *avil);
boolean stQ_push(stQ_Node stQ[],int data,int *tail,int *avil);
boolean stQ_pop(stQ_Node stQ[],int *data,int *tail,int *avil);

#endif


