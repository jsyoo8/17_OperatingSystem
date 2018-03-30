#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

#define MAX 1024
#define STDIN 0 
#define STDOUT 1

int main(int argc, char **argv) {
        char *buf;
        char *fileName;
        int fd;
        int readNum;
	buf = (char *)malloc(MAX);
        fileName = argv[1];
        char *msg = "Input : ";
        memset(buf, 0x00, MAX);
        fd = open(fileName, O_WRONLY | O_CREAT, 0644);
        write(STDOUT, msg, strlen(msg));
        while(((readNum = read(STDIN, buf, MAX-1)) >0)) {
                write(fd, buf, readNum);
		if(buf[0] == '!') {break;}
                write(STDOUT, msg, strlen(msg));
        }
        close(fd);
        return 1;
}
