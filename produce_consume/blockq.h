#include <func.h>

#define N 10

typedef struct blockqueue {
    int elements[N];
    int front;
    int rear;
    int size;
    // not_empty成立, 通知消费者消费
    pthread_cond_t not_empty;
    // not_full成立, 通知生产者生产
    pthread_cond_t not_full;
    pthread_mutex_t mutex;
} blockq_t;

blockq_t* blockq_create();
bool isEmpty(blockq_t *q);
bool isFull(blockq_t *q);
int peek(blockq_t *q);
void enqueue(blockq_t *q, int val);
int dequeue(blockq_t *q);
