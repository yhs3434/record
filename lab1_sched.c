/*
 *	DKU Operating System Lab
 *	    Lab1 (Scheduler Algorithm Simulator)
 *	    Student id : 32131728
 *	    Student name : Yoon Han Sol 
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
    q->head = (Node*)malloc(sizeof(Node));
    q->tail = (Node*)malloc(sizeof(Node));
    q->head->prev=NULL;
    q->head->next=q->tail;
    q->head->data=qNull();
    q->tail->prev=q->head;
    q->tail->next=NULL;
    q->tail->data=qNull();
}

void qPush(Queue *q, qData data)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data=data;

    newNode->prev=q->head;
    newNode->next=q->head->next;
    q->head->next->prev=newNode;
    q->head->next=newNode;
    q->numOfData++;

    return;
}

qData qPop(Queue *q)
{
    if(!q->numOfData){
	printf("Queue is empty\n");
	return qNull();
    }
    Node *rNode = q->tail->prev;
    qData rData = rNode->data;
    rNode->prev->next=rNode->next;
    rNode->next->prev=rNode->prev;
    free(rNode);
    q->numOfData--;

    return rData;
}

void sjfPush(Queue *q, qData data)
{
    if(!q->numOfData){
	qPush(q, data);
	return;
    }

    Node* ptr = q->tail->prev;
    for(;ptr!=q->head;ptr=ptr->prev){
	if(data.serviceTime < ptr->data.serviceTime){
	    Node* newNode = (Node*)malloc(sizeof(Node));
	    newNode->data=data;
	    newNode->prev=ptr;
	    newNode->next=ptr->next;
	    ptr->next->prev=newNode;
	    ptr->next=newNode;
	    q->numOfData++;
	    return;
	}
    }
    qPush(q, data);
}


int getTotalServiceTime(qData* task, int numOfTask)
{
    int i;
    int total=0;
    int min=9999;
    for(i=0;i<numOfTask;i++){
	if(task[i].arrivalTime<min)
	    min=task[i].arrivalTime;
	total+=task[i].serviceTime;
    }
    return min + total;
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
    // int n;

    qData nullData = qNull();
    qData procTask = nullData;

    Queue q;
    qInit(&q);

    int totalServiceTime = getTotalServiceTime(task, numOfTask);

    for(i=0;i<totalServiceTime;i++){
	for(j=0;j<numOfTask;j++){
	    if(task[j].arrivalTime == i){
		sjfPush(&q, task[j]);
	    }
	}

	/*
	   n = 0;
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
	 */ // Amendded prev version

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

void RR(qData task[], int numOfTask)
{
    int i,j;
    int totalServiceTime = getTotalServiceTime(task, numOfTask);
    
    Queue q;
    qInit(&q);

    qData nullData = qNull();
    qData *procTask = &nullData;

    for(i=0;i<totalServiceTime;i++){
	for(j=0;j<numOfTask;j++)
	    if(task[j].arrivalTime == i)
		qPush(&q, task[j]);
	
	if(!checkNull(*procTask) && procTask->serviceTime>0)
	    qPush(&q, *procTask);
	*procTask = qPop(&q);
	printf("%c ", procTask->name);
	procTask->serviceTime--;
    }
}

void RRwithTQ(qData task[], int numOfTask, int timeQuantum)
{
    int i,j;
    int totalServiceTime = getTotalServiceTime(task, numOfTask);

    Queue q;
    qInit(&q);

    qData nullData = qNull();
    qData *procTask = &nullData;
    int pCount = 0;

    for(i=0; i<totalServiceTime ; i++){
	for(j=0;j<numOfTask;j++)
	    if(task[j].arrivalTime == i)
		qPush(&q, task[j]);
	
	if(checkNull(*procTask))
	    *procTask=qPop(&q);
	if(checkNull(*procTask))
	    continue;

	if(procTask->serviceTime==0){
	    procTask=&nullData;
	    pCount=0;
	    *procTask=qPop(&q);
	    if(checkNull(*procTask))
		continue;
	}
	else if(pCount >= timeQuantum){
	    qPush(&q, *procTask);
	    pCount=0;
	    *procTask=qPop(&q);
	}
	
	printf("%c ", procTask->name);
	procTask->serviceTime--;
	pCount++;
    }
}
