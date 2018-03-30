#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define FILE0 "./file0.txt" //write
#define FILE1 "./file1.txt" //read

typedef struct buff {
	int num;
	int pid;
	int com;
	int win;
}buf;

int numbering(void);

sem_t* sem[2];
buf* ptr[2];

int main(void){
	buf* buff[2];
	int fd[2];
	int num;
	int win=0;
	int acc = 0;
	int sval = 2;
	sem_t* semLogin;
	srand(time(NULL));

	buff[0] = (buf*)malloc(sizeof(buf));
	buff[1] = (buf*)malloc(sizeof(buf));
	memset(buff[0], 0x00, sizeof(buf));
	memset(buff[1], 0x00, sizeof(buf));
	if(-1 == (fd[0] = open(FILE0, O_RDWR | O_CREAT, S_IRUSR|S_IWUSR))){
		perror("open error!!!\n");
		exit(1);
	}
	if(-1 == (fd[1] = open(FILE1, O_RDWR | O_CREAT, S_IRUSR|S_IWUSR))){
		perror("open error!!!\n");
		exit(1);
	}

	write(fd[0], buff[0], (sizeof(buf)));
	write(fd[1], buff[1], (sizeof(buf)));
	ptr[0] = (buf*)mmap(0, (sizeof(buf)), PROT_READ | PROT_WRITE, MAP_SHARED, fd[0], 0);
	ptr[1] = (buf*)mmap(0, (sizeof(buf)), PROT_READ | PROT_WRITE, MAP_SHARED, fd[1], 0);
	sem_unlink("sem0");
	if((sem[0]=sem_open("sem0", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("open error!!!\n");
		exit(1);
	}
	sem_unlink("sem1");
	if((sem[1]=sem_open("sem1", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("open error!!!\n");
		exit(1);
	}
	sem_unlink("login");
	if((semLogin=sem_open("login", O_CREAT, 0644, 2)) == SEM_FAILED) {
		perror("open error!!!\n");
		exit(1);
	}
	num=numbering();
	while(sval>0){
		sem_getvalue(semLogin, &sval);
		if(acc == 1){
			printf("client %d hi",sval);
			acc=0;
		}
	}

	printf("========================\n");
	printf("      Game start!!\n");
	printf("========================\n");

	while(1){
		sem_wait(sem[1]);

		printf("input : %d\n",ptr[1]->num);

		if(ptr[1]->num < num){
			printf("[%d] %d result : UP\n", ptr[1]->pid, ptr[1]->num);

			ptr[0]->pid = ptr[1]->pid;
			ptr[0]->num = ptr[1]->num;
			ptr[0]->com = 1;

		}
		else if(ptr[1]->num > num){
			printf("[%d] %d result : DOWN\n", ptr[1]->pid, ptr[1]->num);

			ptr[0]->pid = ptr[1]->pid;
			ptr[0]->num = ptr[1]->num;
			ptr[0]->com = -1;


		}
		else {
			printf("[%d] %d result : Correct\n", ptr[1]->pid, ptr[1]->num);
			printf("%d win!\n", ptr[1]->pid);

			ptr[0]->pid = ptr[1]->pid;
			ptr[0]->num = ptr[1]->num;
			ptr[0]->com = 0;
			sem_post(sem[1]);
			sleep(2);

			break;      


		}

		sem_post(sem[1]);   
		sleep(2);
	}

	sem_close(sem[0]);
	sem_close(sem[1]);
	sem_close(semLogin);
	munmap(ptr[0], (sizeof(buf)));
	munmap(ptr[1], (sizeof(buf)));
	close(fd[0]);
	close(fd[1]);
	unlink(FILE0);
	unlink(FILE1);
	return 0;
}

int numbering(){
	int num=0;
	num = (rand()%100 + 1) ;
	printf("number : %d\n",num);
	return num;
}

