/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 
*	    Student name : 
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
	qData taskArr[]={{'A', 0, 3},{'B', 2, 6},{'C',4,4},{'D',6,5},{'E',8,2}};
	
	struct qData task[Taskss] = { {'A',0,3},{'B',2,6},{'C',4,4},{'D',6,5},{'E',8,2} };

	FCFS(task);
	Round_Robin(task, 1);
	Round_Robin(task, 4);
	printf("============== SJF ==================\n");
	SJF(taskArr, 5);
	// RRwithTQ(taskArr, 5, 1);
	// printf("\n");
	printf("\n============== MLFQ(1) ==============\n");
	MLFQ2(taskArr, 5, 1);
	
	printf("\n\n============== MLFQ(2) ==============\n");
	MLFQ2(taskArr, 5, 2);
	printf("\n\n============== Lottery ==============\n");
	lottery(taskArr, 5);
	

	return 0;
}

