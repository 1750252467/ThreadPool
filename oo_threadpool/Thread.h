/* File Name Thread.h
 * Author 泷fengyoukai
 * Github https://github.com/1750252467
 * Created Time 2023-03-04 19:26
 * Last Modified 2023-03-04 20:03
 */
#ifndef __THREAD_H__
#define __THREAD_H__
#include "NonCopyable.h"
#include <pthread.h>

class Thread
:NonCopyable
{
public:
    Thread();
    virtual ~Thread();
    void start();
    void join();

private:
    static void *threadFunc(void *arg);
    virtual void run() = 0;

private:
    pthread_t _thid;
    bool _isRunning;
};


#endif // __THREAD_H__
