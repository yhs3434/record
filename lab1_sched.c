/*
 *	DKU Operating System Lab
 *	    Lab1 (Scheduler Algorithm Simulator)
 *	    Student id : 
 *	    Student name : 
 *
 *   lab1_sched.c :
 *       - Lab1 source file.
 *       - Must contains scueduler algorithm function'definition.
 *
 */

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */

void qInit(Queue *q)
{
	q->numOfData = 0;
	q->head = NULL;
	q->tail = NULL;
}

void qPush(Queue *q, qData data)
{
	Node *newNode = (Node*)malloc(sizeof(Node));
	newNode->data=data;

	if(!q->numOfData){
		newNode->prev=NULL;
		newNode->next=NULL;
		q->head=q->tail=newNode;
		q->numOfData++;
		return;
	}

	newNode->next=q->head;
	newNode->prev=NULL;
	q->head->prev=newNode;
	q->head=newNode;
	q->numOfData++;

	return;
}

qData qPop(Queue *q)
{
	if(!q->numOfData){
		printf("Queue is empty\n");
		return -1;
	}

	Node *rNode = q->tail;
	qData rData = q->tail->data;
	
	if(q->numOfData==1){
		free(rNode);
		q->numOfData--;
		return rData;
	}
	
	q->tail->prev->next=NULL;
	q->tail=q->tail->prev;
	free(rNode);
	q->numOfData--;

	return rData;
}
