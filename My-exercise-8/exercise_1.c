#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#define SIZE 1
#define NAME "/pesce_molle"



void child_process(double x, int i){
double result;
result=pow(x,i);
printf("\n child number %d, result is %lf", i, result);
*(shma+i)=result;
exit(0);
}



int main(){
FILE *fp
int number,shm_fd;
void *shm_area;
pid_t pid;

pid=fork();
if(pid==0) {
	child_process(x,i);
}else if (pid < 0){
	perror("error fork\n");
}else{
	shm_fd=shm_open(NAME,O_RDWR | O_CREAT, 0666);
	if (shm_fd == -1){
		perror("error shm_open\n");
		exit(-1);
	}
	ftruncate (shm_fd, SIZE);
	printf("shared memory created \n");
	fp= fopen("number.dat", "r");
	if (fp==NULL){
		perror("error FILE\n");
		exit(-1);
	}
	shm_area= mmap(NULL, SIZE, PROT_READ | PROT_WRITE,)
}

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


