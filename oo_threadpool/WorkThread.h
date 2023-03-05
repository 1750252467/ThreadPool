/* File Name WorkThread.h
 * Author 泷fengyoukai
 * Github https://github.com/1750252467
 * Created Time 2023-03-04 20:30
 * Last Modified 2023-03-04 21:07
 */
#ifndef __WORKTHREAD_H__
#define __WORKTHREAD_H__
#include "Thread.h"
#include "ThreadPool.h"

class WorkThread
:public Thread
{
public:
    
    WorkThread(ThreadPool &pool)
    :_pool(pool)
    {

    }

    ~WorkThread() {}

    void run() override 
    {
        _pool.threadFunc();
    }
private:
    ThreadPool &_pool;
};


#endif // __WORKTHREAD_H__
