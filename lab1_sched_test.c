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
	
	qData task1[]={{'A', 0, 7},{'B', 2, 10},{'C',4,3},{'D',6,5},{'E',8,2}};
	qData2 task2[] = {{'A',0,7},{'B',2,10},{'C',4,3},{'D',6,5},{'E',8,2}};

	printf("made by \tNam Hye Min\n\t\tYoon Han Sol\n\n");
	FCFS(task2);
	chart();
	Round_Robin(task2, 1);
	chart();
	Round_Robin(task2, 4);
	chart();
	printf("============== SJF ==================\n");
	SJF(task1, 5, &schedQ);
	printChart(&schedQ);
	printf("\n============== MLFQ(1) ==============\n");
	MLFQ2(task1, 5, 1, &schedQ);
	printChart(&schedQ);
	printf("\n\n============== MLFQ(2) ==============\n");
	MLFQ2(task1, 5, 2, &schedQ);
	printChart(&schedQ);
	printf("\n\n============== Lottery ==============\n");
	lottery(task1, 5, &schedQ);
	printChart(&schedQ);
	

	return 0;
}

