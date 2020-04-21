#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
pid_t pid,ppid;
pid=fork();
if (pid<0){
printf("error\n");
exit(-1);
}
if (pid==0){
printf("I'm child number 1\n");
exit(1);
}else{
ppid=getpid();
printf("I'm parent Pid= %d with son Pid= %d\n", ppid,pid);
pid=fork();
}
if (pid<0){
printf("error\n");
exit(-1);
}
if (pid==0){
printf("I'm child number 2\n");
exit(2);
}else{
ppid=getpid();
printf("I'm parent Pid= %d with son Pid= %d\n", ppid,pid);
pid=fork();
}
if (pid<0){
printf("error\n");
exit(-1);
}
if (pid==0){
printf("I'm child number 3\n");
exit(3);
}else{
ppid=getpid();
printf("I'm parent Pid= %d with son Pid= %d\n", ppid,pid);
}
return 0;

}

