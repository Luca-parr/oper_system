#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include<sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include <string.h>

int main(){
char command[100];
pid_t pid;
char command1[110];
do{

scanf("%s", &command);
if(strcmp(command, "exit")!= 0){
pid=fork();
if (pid<0){
        printf("error\n");
        exit(-1);
        }
if (pid==0){
	printf("%s > %d.log", command, getpid());
	sprintf(command1, "%s > %d.log", command, getpid());
        system(command1);
	exit(0);
}
}
}while(strcmp(command, "exit")!= 0);

return 0;
}
