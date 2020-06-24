#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

double * shma;

void child_process(double x, int i){
double result;
result=pow(x,i);
printf("\n child number %d, result is %lf", i, result);
*(shma+i)=result;
exit(0);
}



int main(){

double x,result=0;
int N,shmid;
int i=0;
pid_t pid;


printf("\n insert N:");
scanf("%d", &N);
if(N>5){
	N=5;
}
printf("\n insert x:");
scanf("%lf", &x);

shmid = shmget(IPC_PRIVATE,sizeof(double)*N,IPC_CREAT | 0666 );
if(shmid<0){
	perror("Error shmget():");
	exit(-1);
}
shma =shmat (shmid,NULL, 0);
if(shma==(void*)-1){
	perror("Error shmat():");
	exit(-1);
}

for (i=0; i<N; i ++) {
	pid=fork();
	if(pid==0) {
		child_process(x,i);
	}
	sleep(1);
}
result=0;

for (i=0; i<N; i ++) {
	wait(NULL);
	result += *(shma+i);
}
printf("\nresult is: %lf", result);
if(shmdt(shma)!=0){
	perror("error detaching: ");
}
shmctl(shmid,IPC_RMID,0);
return 0;
}


