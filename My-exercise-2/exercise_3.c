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
printf("I'm child number 1 with pid= %d and father= %d\n", getpid(), getppid());
pid=fork();
if (pid<0){
printf("error\n");
exit(-1);
}
if (pid==0){
printf("I'm child number 2 with pid= %d and father= %d\n", getpid(), getppid());
pid=fork();
if (pid<0){
printf("error\n");
exit(-1);
}
if (pid==0){
printf("I'm child number 3 with pid= %d and father= %d\n", getpid(), getppid());
}else{
sleep(2);
}
}else{
sleep(2);
}
}else{
sleep(2);
ppid=getpid();
printf("I'm parent Pid= %d with son Pid= %d\n", ppid,pid);
}
return 0;

}

