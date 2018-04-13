/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32131698, 32131728 
*	    Student name : Nam Hye Min, Yoon Han Sol
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
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
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){
    	Queue q;
	qInit(&q);
	Queue schedQ;
	qInit(&schedQ);
	
	qData task1[]={{'A', 0, 7},{'B', 2, 10},{'C',4,3},{'D',6,5},{'E',8,2},{'F',1,4}};
		
	int numOfTask = 6;

	printf("made by \tNam Hye Min\n\t\tYoon Han Sol\n\n");
	FCFS(task1, numOfTask, &schedQ);
	printChart(&schedQ, numOfTask);
	Round_Robin(task1, numOfTask, 1, &schedQ);
	printChart(&schedQ, numOfTask);
	Round_Robin(task1, numOfTask, 4, &schedQ);
	printChart(&schedQ, numOfTask);
	printf("============== SJF ==================\n");
	SJF(task1, numOfTask, &schedQ);
	printChart(&schedQ, numOfTask);
	printf("\n============== MLFQ(q = 1) ==============\n");
	MLFQ2(task1, numOfTask, 1, &schedQ);
	printChart(&schedQ, numOfTask);
	printf("\n============== MLFQ(q = 2^n) ==============\n");
	MLFQ2(task1, numOfTask, 2, &schedQ);
	printChart(&schedQ, numOfTask);
	printf("\n============== Lottery ==============\n");
	lottery(task1, numOfTask, &schedQ);
	printChart(&schedQ, numOfTask);
	

	return 0;
}

