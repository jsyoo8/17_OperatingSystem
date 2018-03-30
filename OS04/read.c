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
        memset(buf, 0x00, MAX);
        fd = open(fileName, O_RDONLY, 0644);
        while (((readNum = read(fd, buf, MAX-1)) >0)) {
                write(STDOUT, buf, readNum);
		if(buf[0] == '!') {break;}
        }
        close(fd);
        return 1;
}
