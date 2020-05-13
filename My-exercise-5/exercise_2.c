
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


char string[21]="";


void* thread_operation (void* arg){

while(strcmp(string, "exit")!=0){
	if((strcmp(string, "casa")==0 || strcmp(string, "house")==0)){
		printf("\n house detected");
	}
	sleep(2);
}
return NULL;
}


int main(){

int thread_return_value;
pthread_t id_thread;
thread_return_value=pthread_create(&id_thread, NULL, &thread_operation, NULL);

if (thread_return_value != 0){
printf("\n cannot create thread : %s", strerror(thread_return_value));
}else {
printf("\n creating thread");
}
do{
printf("\n write string: \n");
scanf("%s", string);
}while(strcmp(string, "exit")!=0);

pthread_join(id_thread,NULL);
printf("\n first thread joined");

return 0;
}


