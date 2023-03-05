/* File Name Condition.h
 * Author 泷fengyoukai
 * Github https://github.com/1750252467
 * Created Time 2023-03-04 18:13
 * Last Modified 2023-03-04 18:17
 */
#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "NonCopyable.h"
#include <pthread.h>
class MutexLock;

class Condition{
public:
    Condition(MutexLock &mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyAll();
private:
    pthread_cond_t _cond;
    MutexLock &_mutex;
};


#endif // __CONDITION_H__
