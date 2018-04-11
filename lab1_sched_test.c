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

#include "lab1_sched_types.h"

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){
    	Queue q;
	qInit(&q);
	qData taskArr[]={{'A', 0, 3},{'B', 2, 6},{'C',4,4},{'D',6,5},{'E',8,2}};
	
	//RRwithTQ(taskArr, 5, 1);
	//lottery(taskArr, 5);
	//MLFQ(taskArr, 5);
	MLFQ2(taskArr, 5, 2);

	return 0;
}

