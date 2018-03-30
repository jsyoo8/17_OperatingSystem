#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFF_SIZE 100

int main(void) {
	int pipe_parent[2];
	int pipe_child[2];
	char buff[BUFF_SIZE];
	pid_t pid;
	int status;
	if(-1 == pipe(pipe_parent)) {
		perror("error parent pipe");
		exit(1);
	}
	if(-1 == pipe(pipe_child)) {
		perror("error child pipe");
		exit(1);
	}
	if((pid=fork()) == 0) {
		sprintf(buff, "(child) test_pipe");
		write(pipe_child[1], buff, strlen(buff));
		memset(buff, 0x00, BUFF_SIZE);

		read(pipe_parent[0], buff, BUFF_SIZE);
		printf("child: %s\n",buff);
	}
	else{
		sprintf(buff, "(parent) test_pipe");
		write(pipe_parent[1], buff, strlen(buff));
		wait(&status);
		memset(buff, 0x00, BUFF_SIZE);

		read(pipe_child[0], buff, BUFF_SIZE);
		printf("parent:%s\n", buff);
	}
	return 0;
}
