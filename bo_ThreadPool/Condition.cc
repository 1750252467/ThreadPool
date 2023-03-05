#include "Condition.h"
#include "MutexLock.h"
#include <stdio.h>

Condition::Condition(MutexLock &mutex)
: _mutex(mutex)
{
    int ret = pthread_cond_init(&_cond, nullptr);
    if(ret)
    {
        perror("pthread_cond_init");
    }
}

Condition::~Condition()
{
    int ret = pthread_cond_destroy(&_cond);
    if(ret)
    {
        perror("pthread_cond_destroy");
    }

}
void Condition::wait()
{
    int ret = pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());
    if(ret)
    {
        perror("pthread_cond_wait");
        return;
    }

}
void Condition::notify()
{
    int ret = pthread_cond_signal(&_cond);
    if(ret)
    {
        perror("pthread_cond_signal");
        return;
    }

}
void Condition::notifyAll()
{
    int ret = pthread_cond_broadcast(&_cond);
    if(ret)
    {
        perror("pthread_cond_broadcast");
        return;
    }

}
