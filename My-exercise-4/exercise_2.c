
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include<sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include <string.h>

int main( int argc, char* argv[]){
char command[100];
if(argc>=3){
sprintf(command, " find %s -name %s", argv[1], argv[2]);
system(command);
}else{
printf("wrong parameters \n");
}
return 0;
}

