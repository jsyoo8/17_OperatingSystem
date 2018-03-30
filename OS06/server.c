#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <wait.h>

#define FIFO1 "./fifo1"
#define FIFO2 "./fifo2"
#define BUFF_SIZE 100

int main(void){
	char buff[BUFF_SIZE];
	pid_t pid;
	int status;
	int fd1;
	int fd2;
	int myPipe[2];
	int temp;
	int num;
	int cnt = 0;
	srand(time(NULL));
	if ( -1 == pipe(myPipe))
	{
		perror( "pipe error!!!\n");
		exit(1);
	}
	if((pid = fork()) == 0){
		memset(buff, 0x00, BUFF_SIZE);
		num = (rand()%100 + 1) ;
		printf("number : %d\n",num);
		sprintf(buff,"%d",num);
		write(myPipe[1], buff, strlen(buff));
		exit(0);
	}
	else{
		wait(&status);
		memset(buff, 0x00, BUFF_SIZE);
		if(-1 == ((mkfifo(FIFO1, 0666) | mkfifo(FIFO2, 0666)))){
			perror("mkfifo error!!!\n");
			exit(1);
		}
		if(-1 == ((fd1 = open(FIFO1, O_WRONLY)) | (fd2 = open(FIFO2,O_RDONLY)))){
			perror("open error!!!\n");
			exit(1);
		}
		read(myPipe[0], buff, BUFF_SIZE);
		num=atoi(buff);
		while(cnt < 10){
			memset(buff, 0x00, BUFF_SIZE);
			read(fd2, buff, BUFF_SIZE);
			cnt ++ ;
			temp = atoi(buff);
			memset(buff, 0x00, BUFF_SIZE);
			printf("answer : %d\n", temp);
			if(temp < num){
				sprintf(buff,"UP");
				write(fd1, buff, strlen(buff));
				printf("result : UP(%d/10)\n",cnt);}
			else if(temp == num){
				sprintf(buff,"CORRECT");
				write(fd1, buff, strlen(buff));

				printf("result : Correct(%d/10)\n",cnt);
				close(fd1);
				close(fd2);

				if(-1 == (unlink(FIFO1) | unlink(FIFO2))){
					perror("unlink error!!!!\n");
					exit(1);
				}
				exit(0);
			}
			else{
				sprintf(buff,"DOWN");
				write(fd1, buff, strlen(buff));

				printf("result : DOWN(%d/10)\n",cnt);
			}


		}
		printf("Failure\n");
		close(fd1);
		close(fd2);

		if(-1 == (unlink(FIFO1) | unlink(FIFO2))){
			perror("unlink error!!!!\n");
			exit(1);
		}
	}
	return 0;
}
