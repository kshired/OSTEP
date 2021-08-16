#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#define MAX 1

sem_t empty;
sem_t full;
sem_t mutex;

int fill = 0;
int use = 0;
int buffer[MAX];

void put(int value) {
  buffer[fill] = value;
  fill = (fill + 1) % MAX;
}

int get() {
  int tmp = buffer[use];
  use = (use + 1) % MAX;
  return tmp;
}

void *producer(void *arg) {
  int i;
  for (i = 0; i < 1e7; ++i) {
    sem_wait(&empty);
    sem_wait(&mutex);
    put(i);
    sem_post(&mutex);
    sem_post(&full);
  }
  return NULL;
}

void *consumer(void *args) {
  int tmp = 0;
  while (tmp != -1) {
    sem_wait(&full);
    sem_wait(&mutex);
    tmp = get();
    sem_post(&mutex);
    sem_post(&empty);
    printf("%d\n", tmp);
  }

  return NULL;
}

int main() {
  sem_init(&mutex, 0, 1);
  sem_init(&empty, 0, MAX);
  sem_init(&full, 0, 0);
  pthread_t p, c;

  pthread_create(&p, NULL, producer, NULL);
  pthread_create(&c, NULL, consumer, NULL);
  return 0;
}

/*
1. 생산자 쓰레드가 먼저 실행
sem_wait(&empty)
-> empty = 0
-> 생산자 쓰레드 sleep X
-> put 수행
-> sem_post(&full)
-> full = 1



2. 소비자 쓰레드가 먼저 실행
sem_wait(&full)
-> full = -1
-> 소비자 쓰레드 sleep
생산자 쓰레드로 context switch
-> sem_wait(&empty);
-> empty = 0
-> 그대로 진행, put 수행
-> sem_post(&full)
-> full = 0
소비자 쓰레드 깨움
-> get 수행
-> sem_post(&empty)
-> empty = 1
*/