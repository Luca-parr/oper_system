#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

float avg;

void process_rand ( int pipe_to_read, int pipe_to_write ){
	float b;
	while (1){
		read(pipe_to_read,&avg,sizeof(float));
		printf("child_start with pid: %d\n", getpid());
		if (avg < 27){
			srand(time(NULL));
			b=rand()%20+10;
			avg=(avg+b)/2;
			printf("child b=%f avg=%f\n", b, avg);		
		
		}
		printf("child_end\n");
		sleep(1);
		write(pipe_to_write,&avg,sizeof(float));	
	}

}

void kill_children () {
	printf("killing child!\n");
	exit(0);
}

int main () {
	int pipe_12[2];
	int pipe_23[2];
	int pipe_31[2];
	float a=20;
	float b;
	int status;
	signal(SIGUSR1,kill_children);
	pid_t pd[2];
	if(pipe(pipe_12)!= 0){
		printf("fanculo_1\n");
		exit(EXIT_FAILURE);	
	}
	if(pipe(pipe_23)!= 0){
		printf("fanculo_2\n");
		exit(EXIT_FAILURE);	
	}
	if(pipe(pipe_31)!= 0){
		printf("fanculo_3\n");
		exit(EXIT_FAILURE);	
	}
	pd[0]=fork();
	if (pd[0]==-1){
		printf("errore_fork\n");
		exit(EXIT_FAILURE);

	}
	if(pd[0]==0) {
//first child process1
		process_rand(pipe_31[0], pipe_12[1]);
	}
	pd[1]=fork();
	if (pd[1]==-1){
		printf("errore_fork\n");
		exit(EXIT_FAILURE);
	}	
	if(pd[1]==0){
//second child process2
		process_rand(pipe_12[0] , pipe_23[1]);
	}
//parent process3
	avg= a;
	write(pipe_31[1], &avg, sizeof(float));
	while (avg < 27){
//write pipe_13[1]
		read(pipe_23[0], &avg, sizeof(float));
		printf("parent start \n");
		if( avg < 27 ){
			srand(time(NULL));
			b=rand()%20+10;
			avg=(avg+b)/2;
			printf("parent b: %f avg: %f\n", b, avg);
			printf("parent end\n");
			sleep(1);
			write(pipe_31[1], &avg, sizeof(float));
		}
//read pipe_32[0]
	}
//kill pd[0] and pd[1]
	kill(pd[0], SIGUSR1);
	waitpid(pd[0],&status, 0);
	if(WIFEXITED(status)>0){
		printf("child_0 exited ok\n");
	}
	kill(pd[1],SIGUSR1);
	waitpid(pd[1],&status,0);
	if(WIFEXITED(status)>0){
		printf("child_1 exited ok\n");
	}
	printf("end\n");
	return 0;
	
}




