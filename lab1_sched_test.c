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
	qPush(&q, 20);
	qPush(&q, 12);
	qPush(&q, 15);
	int i=0;
	qData temp;
	while((temp=qPop(&q))!=-1)
		printf("%d ",temp);
	puts("");
	qPush(&q, 44);
	qPush(&q, 2);
	while((temp=qPop(&q))!=-1)
		printf("%d ",temp);;
	puts("");
	return 0;
}

