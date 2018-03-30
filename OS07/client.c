#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include <semaphore.h>

#define FILE0 "./file0.txt" //read
#define FILE1 "./file1.txt" //write

typedef struct buff {
	int num;
	int pid;
	int com;
	int win;
}buf;

sem_t* sem[2];
buf* ptr[2];

int main(void){
	buf* buff[2];
	int pid;
	int win = 0;
	int num;
    char input[50]={};
	int temp = 0;
	int fd[2];
	int sval = 2;
	int ready = 0;
	sem_t* semLogin;

	buff[0] = (buf*)malloc(sizeof(buf));
	buff[1] = (buf*)malloc(sizeof(buf));
	memset(buff[0], 0x00, sizeof(buf));
	memset(buff[1], 0x00, sizeof(buf));
	if(-1 == (fd[0] = open(FILE0, O_RDWR, S_IRUSR|S_IWUSR))){
		perror("open error!!!\n");
		exit(1);
	}
	if(-1 == (fd[1] = open(FILE1, O_RDWR, S_IRUSR|S_IWUSR))){
		perror("open error!!!\n");
		exit(1);
	}


	ptr[0] = (buf*)mmap(0, sizeof(buf), PROT_READ | PROT_WRITE, MAP_SHARED, fd[0], 0);
	ptr[1] = (buf*)mmap(0, sizeof(buf), PROT_READ | PROT_WRITE, MAP_SHARED, fd[1], 0);
	if((sem[0]=sem_open("sem0", O_CREAT)) == SEM_FAILED) {
		perror("open error!!!\n");
		exit(1);
	}

	if((sem[1]=sem_open("sem1", O_CREAT)) == SEM_FAILED) {
		perror("open error!!!\n");
		exit(1);
	}

	if((semLogin=sem_open("login", O_CREAT)) == SEM_FAILED) {
		perror("open error!!!\n");
		exit(1);
	}

	sem_getvalue(semLogin, &sval);
	if(sval == 0){
		printf("You can't enter this room!\n");
		sem_close(sem[0]);
		sem_close(sem[1]);
		sem_close(semLogin);
		munmap(ptr[0], sizeof(buf));
		munmap(ptr[1], sizeof(buf));
		sem_close(sem[0]);
		sem_close(sem[1]);
		free(buff[0]);
		free(buff[1]);
		exit(0);
	}
	while(sval>0){
		sem_getvalue(semLogin, &sval);
		if(ready == 0){
			sem_wait(semLogin);
			ready=1;
		}
	}
	pid=getpid();

	printf("========================\n");
	printf("      Hello %d\n", pid);
	printf("      Game start!!\n");
	printf("========================\n");


	while(1){
		sem_wait(sem[0]);
		if((ptr[0]->pid)!=0){
			if(ptr[0]->com ==1){
				printf("[%d] %d result : UP\n",ptr[0]->pid, ptr[0]->num);}
			else if(ptr[0]->com ==-1){
				printf("[%d] %d result : DOWN\n",ptr[0]->pid, ptr[0]->num);}
			else {
				printf("[%d] %d result : Correct\n",ptr[0]->pid, ptr[0]->num);
				if(getpid() == ptr[0]->pid){
					printf("You win\n");
					sem_post(semLogin);
					sem_close(sem[0]);
					sem_close(sem[1]);
					sem_close(semLogin);
					munmap(ptr[0], sizeof(buf));
					munmap(ptr[1], sizeof(buf));
					close(fd[0]);
					close(fd[1]);
					free(buff[0]);
					free(buff[1]);
					exit(1);
				}
				else{
					printf("You Lose\n");
					sem_post(semLogin);
					sem_close(sem[0]);
					sem_close(sem[1]);
					sem_close(semLogin);
					munmap(ptr[0], sizeof(buf));
					munmap(ptr[1], sizeof(buf));
					close(fd[0]);
					close(fd[1]);
					free(buff[0]);
					free(buff[1]);
					exit(1);
				}
			}
		}


		sem_wait(sem[1]);   
		while(1){
			printf("input num(1~100) : ");
			fgets(input, 50, stdin);
			input[strlen(input)-1]='\0';
			temp = atoi(input);
			ptr[1]->pid = getpid();
			ptr[1]->num = temp;
			if(temp<=0 || temp >100){
			printf("재입력 [1 ~ 100 사이 숫자]\n");
			printf("_ _ _ _ _ _ _ _ _ _ _ _ _\n");
				continue;   
			}
			break;
		}
		sem_post(sem[1]);

		sleep(1);
		if(ptr[0]->com ==1){
			printf("[%d] %d result : UP\n",ptr[0]->pid, ptr[0]->num);}
		else if(ptr[0]->com ==-1){
			printf("[%d] %d result : DOWN\n",ptr[0]->pid, ptr[0]->num);}
		else {
			printf("[%d] %d result : Correct\n",ptr[0]->pid, ptr[0]->num);
			if(getpid() == ptr[0]->pid){
				sem_post(sem[0]);
				printf("You win\n");
				sem_post(semLogin);
				sem_close(sem[0]);
				sem_close(sem[1]);
				sem_close(semLogin);
				munmap(ptr[0], sizeof(buf));
				munmap(ptr[1], sizeof(buf));
				close(fd[0]);
				close(fd[1]);
				free(buff[0]);
				free(buff[1]);
				exit(1);
			}
			else{
				printf("You Lose\n");
				sem_post(semLogin);
				sem_close(sem[0]);
				sem_close(sem[1]);
				sem_close(semLogin);
				munmap(ptr[0], sizeof(buf));
				munmap(ptr[1], sizeof(buf));
				close(fd[0]);
				close(fd[1]);
				free(buff[0]);
				free(buff[1]);
				exit(1);
			}
		}
		sem_post(sem[0]);
		sleep(1);

	}

	sem_post(semLogin);
	sem_close(sem[0]);
	sem_close(sem[1]);
	sem_close(semLogin);
	munmap(ptr[0], sizeof(buf));
	munmap(ptr[1], sizeof(buf));
	close(fd[0]);
	close(fd[1]);
	free(buff[0]);
	free(buff[1]);
	return 0;
}

