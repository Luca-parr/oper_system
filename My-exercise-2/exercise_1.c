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
printf("I'm child pid= %d", pid);
}else{
ppid=getpid();
printf("I'm parent Pid= %d with son Pid= %d\n", ppid,pid);
}
return 0;

}
