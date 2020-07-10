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
#define NAME_SEM "SH_SEMAPHORE"
#define NAME_NUM "SH_NUMBER"

int main () {
	int num = -1;
	int shm_fd1;
	int shm_fd2;
	int pid;
	sem_t semaphore_1;
	void *shm_sem;
	void *shm_num;
	shm_fd1=shm_open( NAME_SEM, O_CREAT | O_RDWR , 0666 );
	shm_fd2=shm_open( NAME_NUM, O_CREAT | O_RDWR , 0666 );
	if( shm_fd1 < 0 || shm_fd2 < 0 ){
		printf("ERROR SHM_OPEN \n");
		exit(EXIT_FAILURE);	
	}
	ftruncate(shm_fd1, sizeof(sem_t));
	ftruncate(shm_fd2, sizeof(int));
	shm_sem = mmap( NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd1, 0);
	shm_num = mmap( NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd2, 0);
	if( shm_num < 0 || shm_sem < 0 ){
		printf("ERROR MMAP \n");
		exit(EXIT_FAILURE);	
	}
	if(sem_init( &semaphore_1, 1, 1) < 0 ) {
		printf("error semaphor \n");
		exit(EXIT_FAILURE);
	}
	memcpy( shm_sem, &semaphore_1, sizeof(sem_t));
	memcpy( shm_num, &num, sizeof(int));
	pid=fork();
	if(pid!=0){
		while(1){
			srand(time(NULL));
			num = rand()%9+1;
			printf("parent sleepin for %d sec\n", num);
			sleep(num);
			printf("parent up but waiting his turn \n");
			sem_wait(shm_sem);
			printf("parent writes!\n");
			memcpy( shm_num, &num, sizeof(int));
			sleep(5);
			printf("parent_done\n");
			sem_post(shm_sem);
		
		}
	
	} else {
		while(1){
			srand(time(NULL));
			num = rand()%2+1;
			printf("first child with pid: %d sleepin for %d sec\n", getpid(), num);
			sleep(num);
			printf("first_child up and generating process \n");
			pid=fork();
			if (pid==0){
				while(1){
					srand(time(NULL));
					num = rand()%4+1;
					printf("child %d sleepin for %d sec\n", getpid(), num);
					sleep(num);
					printf("child %d up but waiting his turn \n", getpid());
					sem_wait(shm_sem);
					memcpy( &num, shm_num, sizeof(int));
					if(num== -1){
						printf("child %d says it is already num = %d\n", getpid(), num);
						sleep(5);
						printf("child %d done\n", getpid());
						sem_post(shm_sem);
					} else {
						printf("child %d says num = %d so he is gonna change to -1\n", getpid(), num);
						num=-1;
						memcpy( shm_num, &num, sizeof(int));
						sleep(5);
						sem_post(shm_sem);
						printf("child %d exiting\n", getpid());
						exit(0);
					
					}
				}
			}else{
				sleep(5);
				printf("first child done\n");

			}
		}	
	
	}
	
	
	return 0;
	
}




