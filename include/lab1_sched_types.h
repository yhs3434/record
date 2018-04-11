/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 
*	    Student name : 
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H


/*
 * You need to Declare functions in  here
 */

#define TRUE 1
#define FALSE 0

#define MAX 100

typedef struct _data{
    char name;
    int arrivalTime;
    int serviceTime;
}qData;

typedef struct _node{
	qData data;
	struct _node *prev;
	struct _node *next;
}Node;

typedef struct _queue
{
	int numOfData;
	Node *head;
	Node *tail;
}Queue;

typedef qData Task;

typedef struct _lottery_node{
	Task task;
	int ticket;
	struct _lottery_node *prev;
	struct _lottery_node *next;
}LNode;

typedef struct _lottery{
	int numOfProcess;
	int totalTicket;
	LNode *head;
	LNode *tail;
}Lottery;

enum _q
{
	_noQ, _q1, _q2, _q3
};

void qInit(Queue *q);
void qPush(Queue *q, qData data);
qData qPop(Queue *q);
int qIsEmpty(Queue *q);
qData qNull();
int checkNull(qData x);
void sjfPush(Queue *q, qData data);
int getTotalServiceTime(qData* task, int numOfTask);
void quickSort(qData **x, int left, int right);
void sort(qData** task, int numOfTask);

void SJF(qData task[], int numOfTask);
void RR(qData task[], int numOfTask);
void RRwithTQ(qData task[], int numOfTask, int timeQuantum);

void LInit(Lottery *l);
void LInsert(Lottery *l, Task task);
Task LDelete(Lottery *l, Task task);
Task* LVote(Lottery *l);
void lottery(Task task[], int numOfTask);

int getLimitProcTime(int qNum, int tq);
void MLFQ(Task task[], int numOfTask);
void MLFQ2(Task task[], int numOfTask, int tq);

#endif /* LAB1_HEADER_H*/



