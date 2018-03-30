#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO1 "./fifo1"
#define FIFO2 "./fifo2"
#define BUFF_SIZE 100

int main(void)
{
	int cnt = 0;
	int fd1;
	int fd2;
	char buff[BUFF_SIZE];
	int num;

	if( -1 ==((fd1 = open(FIFO1, O_RDONLY)) | (fd2 = open(FIFO2, O_WRONLY))))
	{
		perror("open error!!!!\n");
		exit(1);
	}
	printf("10번 안 에 숫 자 를 알 아 맞 혀 주 세 요 \n");
	while(cnt<10)
	{
		printf("answer : ");
		memset(buff, 0x00, BUFF_SIZE);
		fgets(buff, BUFF_SIZE, stdin);
		buff[strlen(buff)-1]='\0';
		num = atoi(buff);

		if(num<=0 | num>100){
			printf("재입력 [1 ~ 100 사이 숫자]\n");
			printf("_ _ _ _ _ _ _ _ _ _ _ _ _\n");
			continue;
		}
		write(fd2, buff, strlen(buff));
		cnt ++ ;
		memset(buff, 0x00, BUFF_SIZE);
		read(fd1, buff, BUFF_SIZE);
		printf("result : %s(%d/10)\n",buff,cnt);
		if(strncmp(buff,"CORRECT",7)==0){
			close(fd1);
			close(fd2);
			exit(1);
		}
	}
	printf("Failure\n");
	close(fd1);
	close(fd2);
	return 0;
}
