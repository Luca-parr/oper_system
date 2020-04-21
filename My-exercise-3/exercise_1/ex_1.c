#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include<sys/types.h>
#include <unistd.h>
#include<sys/wait.h>

int main(){
pid_t pid,ppid, special_pid;
int status;
for( int i=0; i<2; i++){
	pid=fork();
if (pid<0){
	printf("error\n");
	exit(-1);
	}
if (pid==0){
	printf("child %d with parent %d starts sleeping\n", i+1, getppid());
	sleep(5);
	printf("child %d with parent %d finished sleeping\n", getpid(), getppid());
	exit(1);
}/*else{
		if(i==2){
			special_pid=pid;
		}
	}
}
waitpid(special_pid, &status, 0);
printf("parent %d with child %d finished sleeping\n", getpid(), pid);
return 0;
}*/
}
if(pid=waitpid(pid, &status, 0)){
printf("parent %d with child %d finished sleeping\n", getpid(), pid);
}
pid=wait(&status);
printf("parent %d with child %d finished sleeping\n", getpid(), pid);

return 0;
}

