
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


pthread_t thread_vector[2];


void* thread_operation (void* arg){
pthread_t id=pthread_self();

if(pthread_equal(id,thread_vector[0])){
printf("\n first thread created");

}else{
printf("\n second thread created");
}
sleep(5);
if(pthread_equal(id,thread_vector[0])){
printf("\n finishing first thread");

}else{
printf("\n finishing second thread");
}
return NULL;
}


int main(){

int thread_return_value;
int i=0;

do{
thread_return_value=pthread_create(&thread_vector[i], NULL, &thread_operation, NULL);
if (thread_return_value != 0){
printf("\n cannot create thread : %s", strerror(thread_return_value));
}else {
printf("\n creating thread : %d", i);
}
i++;
}while(i<2);
i=0;
do{

pthread_join(thread_vector[i],NULL);

if (i==0){
printf("\n first thread joined");
}else {
printf("\n second thread joined");
}
i++;
}while(i<2);

return 0;
}


