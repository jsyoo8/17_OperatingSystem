#include<stdio.h>
#include<unistd.h>

int main(){
        char *argv[] = {"ls","-al", NULL};
        char *envp[] = {NULL};

        printf("Running is with execve\n");
        execve("/bin/ls", argv, envp);

        return 0;

}
