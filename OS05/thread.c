#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/sched.h>

#define _GNU_SOURCE

int glob = 0;

int func(int local){
	glob++;
	local++;

	printf("After Thread!!\n");
	printf("Child Process : ");
	printf("getpid=%d, glob=%d, local=%d\n", getpid(), glob, local);

	sleep(1);
	return 0;
}

int main(void){
	int local = 100;
	int child_stack[4096];

	printf("Before Thread!!\n");
	printf("getpid=%d, glob=%d, local=%d\n", getpid(), glob, local);
	
	clone(func, (void*)(child_stack+4096), CLONE_VM | CLONE_THREAD | CLONE_SIGHAND, local);
	sleep(1);
	
	printf("Parent Process : ");
	printf("getpid=%d, glob=%d, local=%d\n", getpid(), glob, local);

	return 0;
}
