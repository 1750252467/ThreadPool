#include "blockq.h"

blockq_t * blockq_create() {
    blockq_t *q = (blockq_t *)calloc(1, sizeof(blockq_t));
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
    return q;
}

bool isEmpty(blockq_t* q) {
    pthread_mutex_lock(&q->mutex);
    bool flag = (q->front == q->rear);
    pthread_mutex_unlock(&q->mutex);
    return flag;
}

bool isFull(blockq_t* q) {
    pthread_mutex_lock(&q->mutex);
    bool flag = (q->rear + 1) % N == q->front;
    pthread_mutex_unlock(&q->mutex);
    return flag;
}

int peek(blockq_t* q) {
    pthread_mutex_lock(&q->mutex);
    while (q->rear == q->front) {
        // 等待队列不为空，并且释放锁, 当wait函数返回时，已经重新获取锁了
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }
    int retval = q->elements[q->front];
    pthread_mutex_unlock(&q->mutex);
    return retval;
}

void enqueue(blockq_t* q, int val) {
    pthread_mutex_lock(&q->mutex);
    while ((q->rear + 1) % N == q->front) {
        // wait的含义不是说，返回的时候条件一定成立，只是说曾经某一时刻成立过
        pthread_cond_wait(&q->not_full, &q->mutex);
    }
    q->elements[q->rear] = val;
    q->rear = (q->rear + 1) % N;
    q->size++;
    // 队列不为空, 唤醒等待 not_empty 的线程
    // 至少会唤醒一个线程，可能唤醒多个
    // signal 和 broadcast 不会释放锁
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);
}

int dequeue(blockq_t* q) {
    pthread_mutex_lock(&q->mutex);
    while (q->rear == q->front) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }
    int retval = q->elements[q->front];
    q->front = (q->front + 1) % N;
    q->size--;
    // 唤醒等待 not_full 的线程
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
    return retval;
}


