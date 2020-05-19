
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct thread_param{
int start;
int last;
char * gen;
char * seq;
};


void* thread_operation (void* arg){
int i;
struct thread_param *p = (struct thread_param *) arg;
for(i=p->start;i<=p->last;i++){
if(strncmp(p->gen+i,p->seq,strlen(p->seq))==0){
printf("\n match found from position %d to position %d", i, i+ (int) strlen(p->seq));
}
}
}


int main(){
int start,step,P;
int thread_return_value;
int i=0;
char name_file_genome[15];
char name_file_seq[15];
char genome[50001];
char sequence[50001];
FILE *fgenom, *fseq;
printf("\n insert name file of genome:");
scanf("%s", name_file_genome);
printf("\n insert name file of sequence:");
scanf("%s", name_file_seq);
//salvo stringhe col nome di file
fgenom = fopen(name_file_genome,'r');
if(fgenom==NULL){
printf("\nerror opening file genom");
exit(1);
}
fseq = fopen(name_file_seq,'r');
if(fseq==NULL){
printf("\nerror opening file seq");
exit(1);
}
fscanf(fgenom, "%s", genome);
fscanf(fseq, "%s", sequence);
//fine file


do{
printf("\ninserire numero di thread valido");
scanf("%d", &P);
}while(P>strlen(genome)-strlen(sequence));

pthread_t thread_vector[P];
struct thread_param par[P];
start=0;
step=strlen(genome)-strlen(sequence)/P;

for (i=0; i<P; i ++) {
par[i].start=start;
if(i<P-1) {
par[i].last=start+step-1;
}else {
par[i].last=strlen(genome)-strlen(sequence);
}
par[i].gen=genome;
par[i].seq=sequence;
thread_return_value=pthread_create(&thread_vector[i], NULL, &thread_operation,(void*) &par[i]);
if (thread_return_value != 0){
printf("\n cannot create thread : %s", strerror(thread_return_value));
exit(1);
}
start=start+step;
}

for (i=0; i<P; i ++) {

pthread_join(thread_vector[i],NULL);
}
return 0;
}


