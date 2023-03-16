#include "blockq.h"


void *produce(void *arg) {
    // 生产线程的入口的函数
    blockq_t *q = (blockq_t *)arg;
    srand(time(NULL));
    while(1) {
        int r = rand();
        enqueue(q, r);
        printf(">> producer %ld: produce a value(%d)\n", pthread_self(), r);
        sleep(1);
    }
    return NULL;
}

void *consume(void *arg) {
    blockq_t *q = (blockq_t *)arg;
    while (1) {
        int v = dequeue(q);
        printf(">> consumer %ld: consume a value(%d)\n", pthread_self(), v);
        sleep(1);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid1, tid2, tid3;
    int err; 
    blockq_t *q = blockq_create();
    err = pthread_create(&tid1, NULL, produce, q);    
    THREAD_ERROR_CHECK(err, "pthread_create");

    err = pthread_create(&tid2, NULL, produce, q);    
    THREAD_ERROR_CHECK(err, "pthread_create");

    err = pthread_create(&tid3, NULL, consume, q);    
    THREAD_ERROR_CHECK(err, "pthread_create");
    
    void * tret;
    err = pthread_join(tid1, &tret);
    THREAD_ERROR_CHECK(err, "pthread_join");

    err = pthread_join(tid2, &tret);
    THREAD_ERROR_CHECK(err, "pthread_join");

    err = pthread_join(tid3, &tret);
    THREAD_ERROR_CHECK(err, "pthread_join");
    return 0;
}

