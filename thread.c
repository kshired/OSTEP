#include <stdio.h>
#include <stdlib.h>
#include "common.h"

// 변수를 최적화에서 제외하여 항상 메모리에 접근하도록 만듦
volatile int counter = 0;
int loops;

void *worker (void *arg){
  int i;
  for(i=0;i<loops;++i){
    counter++;
  }
  return NULL;
}

int main(int argc, char *argv[]){
  if(argc!=2){
    fprintf(stderr, "usage: threads <value>\n");
    exit(1);
  }
  loops = atoi(argv[1]);
  pthread_t p1, p2;

  printf("Initial value : %d\n", counter);
  Pthread_create(&p1, NULL, worker, NULL);
  Pthread_create(&p2, NULL, worker, NULL);
  Pthread_join(p1,NULL);
  Pthread_join(p2,NULL);
  printf("Final value   : %d\n",counter);
  return 0;
}