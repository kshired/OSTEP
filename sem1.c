#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

sem_t s;

void *child(void *arg){
  printf("child\n");
  sem_post(&s);
  return NULL;
}

int main(){
  sem_init(&s,0,0);
  printf("parent : begin\n");
  pthread_t c;
  pthread_create(&c,NULL,child, NULL);
  sem_wait(&s);
  printf("parent: end\n");
  return 0;
}

/*
1. 부모 쓰레드가 먼저 실행
sem_wait 을 통해 세마포어의 값이 0보다 작아짐. ( -1 )
부모 쓰레드 sleep
-> c 쓰레드 실행
-> sem_post를 통해 세마포어 값 증가, 부모 쓰레드가 sleep되어있으니 깨움.

2. 자식 쓰레드가 먼저 실행
sem_post를 통해 세마포의 값이 0보다 커짐. ( 1 )
깨울 쓰레드는 없음. 그냥 부모 쓰레드로 context switch.
-> sem_wait을 했지만, 세마포어의 값이 0이 되기때문에 sleep 되지 않음.
-> 그대로 실행.
*/