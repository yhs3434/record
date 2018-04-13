/**
 *	DKU Operating System Lab
 *	    Lab1 (Scheduler Algorithm Simulator)
 *	    Student id : 32131728, 32131698
 *	    Student name : Yoon Han Sol , Nam Hye Min
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
#include <stdbool.h>

#include "lab1_sched_types.h"

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */

// static variable
int at[Taskss], st[Taskss], rq[50] = { 0, },stm[50] = { 0, };
int node = 0;

void swap(struct _qData *a, struct _qData *b)
{
    struct _qData task;

    task = *a;
    *a = *b;
    *b = task;
}

void ATsort(struct _qData task[])
{
    bool swapped;

    do
    {
	swapped = false;
	for (int count = 0; count<(Taskss - 1); count++)
	{
	    if (task[count].at>task[count+1].at)
	    {
		swap(&task[count], &task[count + 1]);
		swapped = true;
	    }
	}
    } while (swapped);
}

void FCFS(struct _qData *task)
{
    int x, y;

    ATsort(task);

    printf("==================FCFS================== \n");

    for (x = 0; x < Taskss; x++)
    {
	for (y = 0; y < task[x].st; y++)
	{
	    printf("%c ", task[x].name);
	}
    }
    printf("\n\n");
}

// RR use SeachStack,AddQue
void Round_Robin(struct _qData *task,int qt){

    struct _qData temp[Taskss];
    int servicetime;
    int time = 0;
    int pnt = 0; //pnt = process name table, 0~1 = A ~ E 
    int flag=0;

    memcpy(temp, task, sizeof(struct _qData)*Taskss);

    printf("================RRwithTQ(%d)================= \n",qt);

    for (int x = 0; x<Taskss; x++) {
	at[x] = temp[x].at; //at = task's arrival time
	st[x] = temp[x].st; //st = task's service time
	stm[x] = temp[x].st; //stm = service time management
    }
    do {
	if (flag == 0) {
	    servicetime = at[0];
	    //---Reduce service time
	    if (stm[0] <= qt) {
		time = servicetime + stm[0];
		stm[0] = 0;
		SearchStack01(pnt, time);
	    }
	    else {
		stm[0] = stm[0] - qt;
		time = servicetime + qt;
		SearchStack01(pnt, time); 
		AddQue(pnt);
	    }
	}//if
	else {
	    pnt = rq[0] - 1; //rq = reday Que
	    servicetime = time;
	    //---DeleteQue
	    for (int x = 0; x<node && node != 1; x++) {
		rq[x] = rq[x + 1];
	    }
	    node--;
	    //---Reduce service time
	    if (stm[pnt] <= qt) {
		time = servicetime + stm[pnt];
		stm[pnt] = 0;
		SearchStack02(pnt, time);
	    }
	    else {
		stm[pnt] = stm[pnt] - qt;
		time = servicetime + qt;
		SearchStack02(pnt, time);
		AddQue(pnt);
	    }
	}//else

	flag++;

	for (int i=0; i < qt; i++)
	{
	    temp[pnt].st--;
	    if(temp[pnt].st >= 0){
		printf("%c ", temp[pnt].name);
	    }
	}

    } while (node != 0);
    printf("\n\n");
}

void SearchStack01(int pnt, int time) {
    for (int x = pnt + 1; x<5; x++) {
	if (at[x] <= time) {
	    rq[node] = x + 1;
	    node++;
	}
    }
}

void SearchStack02(int pnt, int time) {
    for (int x = pnt + 1; x<5; x++) {
	//---CheckQue
	int fl = 0;
	for (int y = 0; y<node; y++) {
	    if (rq[y] == x + 1) {
		fl++;
	    }
	}
	if (at[x] <= time && fl == 0 && stm[x] != 0) {
	    rq[node] = x + 1;
	    node++;
	}
    }
}

void AddQue(int pnt) {
    rq[node] = pnt + 1;
    node++;
}



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
	// printf("Queue is empty\n");
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

int qIsEmpty(Queue *q)
{
    if(q->head->next == q->tail)
	return TRUE;
    else
	return FALSE;
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

void SJF(qData task[], int numOfTask, Queue *schedQ)
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

	if(checkNull(procTask)){
	    procTask = qPop(&q);
	    printf("%c ", procTask.name);
	    qPush(schedQ, procTask);
	    if(!(--procTask.serviceTime))
		procTask = nullData;
	}
	else{
	    printf("%c ", procTask.name);
	    qPush(schedQ, procTask);
	    if(!(--procTask.serviceTime))
		procTask = nullData;
	}
    }
    printf("\n");
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

void LInit(Lottery *l)
{
    l->head = (LNode*)malloc(sizeof(LNode));
    l->tail = (LNode*)malloc(sizeof(LNode));
    l->head->prev=NULL;
    l->head->next=l->tail;
    l->tail->prev=l->head;
    l->tail->next=NULL;
    l->numOfProcess=0;
    l->totalTicket=0;
}

void LInsert(Lottery *l, Task task)
{
    int ticket = task.serviceTime * 10;
    LNode *newNode = (LNode*)malloc(sizeof(LNode));
    newNode->task = task;
    newNode->ticket = ticket;
    newNode->prev=l->head;
    newNode->next=l->head->next;
    l->head->next->prev=newNode;
    l->head->next=newNode;
    l->numOfProcess++;
    l->totalTicket += ticket;
}

Task LDelete(Lottery *l, Task task)
{
    if(!l->numOfProcess){
	printf("empty\n");
	return qNull();
    }

    LNode *rNode, *cur;
    Task rTask;

    for(cur=l->head->next;cur!=l->tail;cur=cur->next)
	if(cur->task.name == task.name)
	    rNode = cur;
    rNode->prev->next=rNode->next;
    rNode->next->prev=rNode->prev;
    rTask = rNode->task;

    l->totalTicket -= rNode->ticket;
    l->numOfProcess--;
    free(rNode);

    return rTask;
}

Task* LVote(Lottery *l)
{
    int randomValue = ((int)rand() % l->totalTicket) + 1;
    int sumTicket = 0;
    LNode *cur;

    for(cur = l->head->next;cur!=l->tail;cur=cur->next){
	sumTicket += cur->ticket;
	if(randomValue <= sumTicket)
	    break;
    }

    return &(cur->task);
}	

void lottery(Task task[], int numOfTask, Queue* schedQ)
{
    int i, j;

    srand(time(NULL));

    Lottery l;
    LInit(&l);

    Task nullData = qNull();
    Task *procTask = &nullData;

    for(i=0;i<MAX;i++){
	for(j=0;j<numOfTask;j++){
	    if(task[j].arrivalTime == i){
		LInsert(&l, task[j]);
	    }
	}
	if(!l.numOfProcess)
	    continue;
	procTask=LVote(&l);
	if(checkNull(*procTask))
	    continue;
	printf("%c ",procTask->name);
	qPush(schedQ, *procTask);
	if(!(--procTask->serviceTime))
	    LDelete(&l, *procTask);
    }
    printf("\n");
}

int getLimitProcTime(int qNum, int tq)
{
    if(qNum==_q1)
	return 1;
    else if(qNum==_q2)
	return tq;
    else if(qNum==_q3)
	return tq*tq;
    else
	return FALSE;
}

void MLFQ2(Task task[], int numOfTask, int tq, Queue* schedQ)
{
    int i,j;
    Queue q1, q2, q3;
    qInit(&q1);
    qInit(&q2);
    qInit(&q3);

    Task nullData = qNull();
    Task procTask = nullData;
    int procTime = 0;
    int qNum = _noQ;
    int checkNextArrival;

    for(i=0; i<MAX;i++){
	for(j=0; j<numOfTask; j++){
	    if(task[j].arrivalTime == i){
		qPush(&q1, task[j]);
	    }
	}

	if(checkNull(procTask)){
	    if(checkNull(procTask=qPop(&q1))){
		if(checkNull(procTask=qPop(&q2))){
		    if(checkNull(procTask=qPop(&q3))){
			continue;
		    }else{ qNum=_q3; }
		}else{ qNum=_q2; }
	    }else{ qNum=_q1; }
	}

	printf("%c ", procTask.name);
	qPush(schedQ, procTask);
	procTime++;

	if(!(--procTask.serviceTime)){
	    procTask = nullData;
	    qNum=_noQ;
	}
	else if(procTime<(getLimitProcTime(qNum, tq))){

	}
	else{
	    checkNextArrival=FALSE;
	    for(j=0; j<numOfTask; j++)
		if(task[j].arrivalTime==i+1){
		    checkNextArrival=TRUE;
		    break;
		}
	    // If there is not any task in next time
	    if(qNum==_q1){
		if(!checkNextArrival && qIsEmpty(&q1) && qIsEmpty(&q2) && qIsEmpty(&q3))
		    qPush(&q1, procTask);
		else
		    qPush(&q2, procTask);
	    }       
	    else if(qNum==_q2){
		if(!checkNextArrival && qIsEmpty(&q2) && qIsEmpty(&q3))
		    qPush(&q2, procTask);
		else
		    qPush(&q3, procTask);
	    }   
	    else
		qPush(&q3, procTask);
	    procTask = nullData;
	    procTime = 0;
	    qNum=_noQ;
	}
    }
    printf("\n");
}

void chartInit(Chart (*chart)[MAX], int row, int column)
{
    int i,j;
    for(i=0; i<row; i++){
	for(j=0; j<column; j++){
	    chart[i][j].check = FALSE;
	}
    }
}

void chartPush(Chart (*chart)[MAX], char process, int index)
{
    int row;
    if(process == 'A')
	row=0;
    else if(process == 'B')
	row=1;
    else if(process == 'C')
	row=2;
    else if(process == 'D')
	row=3;
    else
	row=4;

    chart[row][index].check = TRUE;
}

void printChart(Queue* schedQ)
{
    int i, j;
    Chart chart[5][MAX];
    chartInit(chart, 5, MAX);
    char process;
    printf("\nChart\n");
    int index = 0;
    while(!qIsEmpty(schedQ)){
	process = qPop(schedQ).name;
	chartPush(chart, process, index);
	index++;
    }
    for(i=0; i<5; i++){
	for(j=0; j<index; j++){
	    if(chart[i][j].check)
		printf("■ ");
	    else
		printf("□ ");
	}
	printf("\n");
    }
}
