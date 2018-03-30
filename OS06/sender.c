#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_FILE "./fifo_temp"
#define BUFF_SIZE 100

int main(void) {
	int cnt = 0;
	int fd;
	char buff[BUFF_SIZE];

	if(-1 == (fd=open(FIFO_FILE, O_WRONLY))) {
		perror("open error!!!\n");
		exit(1);
	}
	while(1) {
		memset(buff, 0x00, BUFF_SIZE);
		fgets(buff, BUFF_SIZE, stdin);
		write(fd, buff, strlen(buff));
	}
	close(fd);
	return 0;
}
