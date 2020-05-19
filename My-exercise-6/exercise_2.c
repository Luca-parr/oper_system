
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct thread_param{
int start;
int last;
int N;
};

int no_prime=0;

void* thread_operation (void* arg){
int i;
struct thread_param *p = (struct thread_param *) arg;
for(i=p->start;i<=p->last&&no_prime==0;i++){
if(p->N % i ==0){
no_prime=1;
pthread_exit(0);
}
}
pthread_exit(0);
}


int main(){
int start,step,P;
int thread_return_value;
int i=0;
int N;

printf("\n insert number:");
scanf("%d", &N);


do{
printf("\ninserire numero di thread valido");
scanf("%d", &P);
}while(P>(N/2)-2);

pthread_t thread_vector[P];
struct thread_param par[P];
start=2;
step=((N/2)-2)/P;

for (i=0; i<P; i ++) {
par[i].start=start;
if(i<P-1) {
par[i].last=start+step-1;
}else {
par[i].last=N/2;
}
par[i].N=N;
thread_return_value=pthread_create(&thread_vector[i], NULL, &thread_operation,(void*) &par[i]);
if (thread_return_value != 0){
printf("\n cannot create thread : %s", strerror(thread_return_value));
exit(1);
}
start=start+step;
}

for (i=0; i<P; i ++) {
pthread_join(thread_vector[i],NULL);
if(no_prime==1){
printf("\nnumero non primo.");
}
}
if(no_prime==0){
printf("\nnumero primo");
}

return 0;
}


