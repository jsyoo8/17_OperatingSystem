#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int glob = 0;

int main ( void ) {
	pid_t var_pid;

	int local = 100;
	int status;
	
	printf("Before Fork!!\n");

	printf("var_pid=%d, getpid=%d, glob=%d, local=%d\n", var_pid, getpid(), glob, local);

	var_pid = fork();

	if(var_pid == 0){
		printf("After Fork!!\n");
		printf("Child Process : ");
		glob++;
		local++;
	}
	else{
		wait(&status);
		printf("Parent Process : ");
	}

	printf("var_pid=%d, getpid=%d, glob=%d, local=%d\n", var_pid, getpid(), glob, local);

	return 0;
}
