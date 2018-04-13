/**
 *	DKU Operating System Lab
 *	    Lab1 (Scheduler Algorithm Simulator)
 *	    Student id : 32131698, 32131728
 *	    Student name : Nam Hye Min, Yoon Han Sol
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


/**
 * =============================================
 *					      =
 *	made by : 32131698  Nam Hye Min	      =
 *					      =
 * =============================================
 **/

// static variable
int at[Taskss], st[Taskss], rq[50] = { 0, },stm[50] = { 0, };
int node = 0;
int arr[20];
int num = 0;

void arrinit()
{
    num = 0;
    for (int i = 0; i < 20; i++)
    {
	arr[i] = 0;
    }
}

void chart() 
{
    int x;
    int A[20], B[20], C[20], D[20], E[20];
    int full = 1;
    int empty = 0;

    for (x = 0; x < 20; x++) 
    {
	if (arr[x] == 'A')
	{
	    A[x] = full;
	    B[x] = empty;
	    C[x] = empty;
	    D[x] = empty;
	    E[x] = empty;
	}
	if (arr[x] == 'B')
	{
	    A[x] = empty;
	    B[x] = full;
	    C[x] = empty;
	    D[x] = empty;
	    E[x] = empty;
	}
	if (arr[x] == 'C')
	{
	    A[x] = empty;
	    B[x] = empty;
	    C[x] = full;
	    D[x] = empty;
	    E[x] = empty;
	}
	if (arr[x] == 'D')
	{
	    A[x] = empty;
	    B[x] = empty;
	    C[x] = empty;
	    D[x] = full;
	    E[x] = empty;
	}
	if (arr[x] == 'E')
	{
	    A[x] = empty;
	    B[x] = empty;
	    C[x] = empty;
	    D[x] = empty;
	    E[x] = full;
	}
    }
    printf("A ");
    for (int y = 0; y < 20; y++)
    {
	if (A[y] == empty)printf("□ ");
	else printf("■ ");
    }
    printf("\nB ");
    for (int y = 0; y < 20; y++)
    {
	if (B[y] == empty)printf("□ ");
	else printf("■ ");
    }
    printf("\nC ");
    for (int y = 0; y < 20; y++)
    {
	if (C[y] == empty)printf("□ ");
	else printf("■ ");
    }
    printf("\nD ");
    for (int y = 0; y < 20; y++)
    {
	if (D[y] == empty)printf("□ ");
	else printf("■ ");
    }
    printf("\nE ");
    for (int y = 0; y < 20; y++)
    {
	if (E[y] == empty)printf("□ ");
	else printf("■ ");
    }
    printf("\n\n");
}

void swap(qData *a, qData *b)
{
    qData task;

    task = *a;
    *a = *b;
    *b = task;
}

void ATsort(qData task[])
{
    bool swapped;

    do
    {
	swapped = false;
	for (int count = 0; count<(Taskss - 1); count++)
	{
	    if (task[count].arrivalTime>task[count+1].arrivalTime)
	    {
		swap(&task[count], &task[count + 1]);
		swapped = true;
	    }
	}
    } while (swapped);
}

void FCFS(qData *task, Queue* schedQ)
{
    int x, y;

    ATsort(task);

    arrinit();

    printf("==================FCFS================== \n");

    for (x = 0; x < Taskss; x++)
    {
	for (y = 0; y < task[x].serviceTime; y++)
	{
	    printf("%c ", task[x].name);
	    qPush(schedQ, task[x]);
	    arr[num] = task[x].name;
	    num++;
	}
    }
    printf("\n\n");
}

// RR use SeachStack,AddQue
void Round_Robin(qData *task,int qt, Queue* schedQ){
    qData temp[Taskss];
    int servicetime;
    int time = 0;
    int pnt = 0; //pnt = process name table, 0~1 = A ~ E 
    int flag=0;

    arrinit();

    memcpy(temp, task, sizeof(qData)*Taskss);

    printf("================RRwithTQ(q = %d)================= \n",qt);

    for (int x = 0; x<Taskss; x++) {
	at[x] = temp[x].arrivalTime; //arrivalTime = task's arrival time
	st[x] = temp[x].serviceTime; //serviceTime = task's service time
	stm[x] = temp[x].serviceTime; //stm = service time management
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
	    temp[pnt].serviceTime--;
	    if(temp[pnt].serviceTime >= 0){
		printf("%c ", temp[pnt].name);
		qPush(schedQ, temp[pnt]);
		arr[num] = temp[pnt].name;
		num++;
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







/**
 * =============================================
 *					      =
 *	made by : 32131728  Yoon Han Sol      =
 *					      =
 * =============================================
 **/	


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

    qData nullData = qNull();
    qData procTask = nullData;

    Queue q;
    qInit(&q);

    for(i=0;i<MAX;i++){
	for(j=0;j<numOfTask;j++){
	    if(task[j].arrivalTime == i){
		sjfPush(&q, task[j]);
	    }
	}

	if(checkNull(procTask)){
	    if(checkNull(procTask = qPop(&q)))
		continue;
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

void chartPush(Chart (*chart)[MAX], char process, int index, int numOfTask)
{
    int row;

    for(row=0; row<numOfTask; row++){
	if(process == ('A'+row)){
	    break;
	}
    }

    chart[row][index].check = TRUE;
}

void printChart(Queue* schedQ, int numOfTask)
{
    int i, j;
    Chart chart[numOfTask][MAX];
    chartInit(chart, numOfTask, MAX);
    char process;
    printf("\n");
    int index = 0;
    while(!qIsEmpty(schedQ)){
	process = qPop(schedQ).name;
	chartPush(chart, process, index, numOfTask);
	index++;
    }
    for(i=0; i<numOfTask; i++){
	printf("%c ",'A'+i);
	for(j=0; j<index; j++){
	    if(chart[i][j].check)
		printf("■ ");
	    else
		printf("□ ");
	}
	printf("\n");
    }
}
