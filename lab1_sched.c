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
	return qNull();
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

int getTotalServiceTime(qData* task, int numOfTask)
{
    int i;
    int total=0;
    for(i=0;i<numOfTask;i++)
	total+=task[i].serviceTime;
    return total;
}

void quickSort(qData **x, int left, int right)
{
    int i=left;
    int j=right;
    int pivot = (*x)[(left+right)/2].serviceTime;
    qData temp;

    while(i<=j){
	while((*x)[i].serviceTime < pivot)
	    i++;
	while((*x)[j].serviceTime > pivot)
	    j--;

	if(i<=j){
	    temp=(*x)[i];
	    (*x)[i]=(*x)[j];
	    (*x)[j]=temp;
	    i++;
	    j--;
	}
    }

    if(left < j)
	quickSort(x, left, j);
    if(i < right)
	quickSort(x, i, right);
}

void sort(qData** task, int numOfTask)
{
    quickSort(task, 0, numOfTask-1);
}

qData qNull()
{
    qData nullData;
    memset(&nullData, 0, sizeof(qData));
    
    return nullData;
}

int checkNull(qData x)
{
    qData qNULL = qNull();
    if(x.name==qNULL.name && x.serviceTime==qNULL.serviceTime && x.arrivalTime==qNULL.arrivalTime)
	return TRUE;
    else
	return FALSE;
}

void SJF(qData task[], int numOfTask)
{
    int i, j;
    int n;
    qData nullData = qNull();
    qData procTask = nullData;

    Queue q;
    qInit(&q);

    int totalServiceTime = getTotalServiceTime(task, numOfTask);

    for(i=0;i<totalServiceTime;i++){
	n=0;
	for(j=0;j<numOfTask;j++)
	    if(task[j].arrivalTime == i)
		n++;
	if(!n){}
	else{
	    sort(&task, numOfTask);
	    for(j=0; j<numOfTask; j++){
		if(task[j].arrivalTime == i && task[j].serviceTime){
		    qPush(&q, task[j]);
		}
	    }
	}

	if(checkNull(procTask)){
	    procTask = qPop(&q);
	    printf("%c ", procTask.name);
	    if(!(--procTask.serviceTime))
		procTask = nullData;
	}
	else{
	    printf("%c ", procTask.name);
	    if(!(--procTask.serviceTime))
		procTask = nullData;
	}
    }
    printf("\n");
}
