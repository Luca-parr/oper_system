#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>
#define FILENAME "numbers.dat"

void *avg_program(int pipe_to_read, int pipe_to_write){
	int count = 1;
	float tst,avg = 0;
	int value;
	while(1) {
		read(pipe_to_read, &value, sizeof(int));
		if (count==1){
			avg=(float) value;
		} else {
			avg= ((float) value + (float) (avg * (count-1)))/( (float) count);
		}
		count++;
		write(pipe_to_write, &avg, sizeof(float));
	}
}



int main () {
	float avg_ev;
	float avg_od;
	int pipe_even[2];
	int avg_even [2];
	int pipe_odd[2];
	int avg_odd [2];
	int num_read;
	pid_t pid[0];
	int status;
	FILE *fd;
	fd = fopen( FILENAME, "r");
	if(fd < 0){
		printf("error file_open\n");
		exit(-1);
	}
	if(pipe(pipe_even)){
		printf("error pipe_even\n");
		exit(-1);
	}
	if(pipe(pipe_odd)){
		printf("error pipe odd\n");
		exit(-1);
	}
	if(pipe(avg_even)){
		printf("error pipe avg_even\n");
		exit(-1);
	}
	if(pipe(avg_odd)){
		printf("error pipe avg odd\n");
		exit(-1);
	}
	pid[0]=fork();
	if(pid[0]==0){
		avg_program(pipe_even[0], avg_even[1]);	
	}
	pid[1]=fork();
	if(pid[1]==0){
		avg_program(pipe_odd[0], avg_odd[1]);
	}
	while(1){
		if(fscanf(fd, "%d", &num_read)!= EOF) {
		if((num_read%2)==0){
			write(pipe_even[1], &num_read, sizeof(int));
			read(avg_even[0], &avg_ev, sizeof(float));
		}else{
			write(pipe_odd[1], &num_read, sizeof(int));
			read(avg_odd[0], &avg_od, sizeof(float));
		}
		printf("avg_even: %f avg_odd: %f\n", avg_ev, avg_od);
		} else {
			close(pipe_even[0]);
			close(pipe_even[1]);
			close(pipe_odd[0]);
			close(pipe_odd[1]);
			close(avg_even[0]);
			close(avg_even[1]);
			close(avg_odd[0]);
			close(avg_odd[1]);			
			kill(pid[0], SIGTERM);
			waitpid(pid[0], &status, 0);
			kill(pid[1], SIGTERM);
			waitpid(pid[1], &status, 0);
			printf("all_child_exited!\nexiting also parent\nThank for using!\n");
			return 0;
		}
	
	
	}

	return 0;
	
}




