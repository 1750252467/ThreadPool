#include "Thread.h"
#include <stdio.h>

Thread::Thread()
: _thid(0)
, _isRunning(false)
{

}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_thid);
        _isRunning = false;
    }
}

//pth->start();
void Thread::start()
{
    //shift + k进行跳转，找到函数的定义
    //threadFunc要设计成静态成员函数，消除this指针影响
    /* int ret = pthread_create(&_thid, nullptr, threadFunc, nullptr); */
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret)
    {
        perror("pthread_create");
    }
    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
    {
        int ret = pthread_join(_thid, nullptr);
        if(ret)
        {
            perror("pthread_join");
            return;
        }
        _isRunning = false;
    }
}
void *Thread::threadFunc(void *arg)
{
    Thread *pth = static_cast<Thread *>(arg);
    //run是非静态的，而threadFunc是静态的
    if(pth)
    {
        pth->run();//体现多态
    }

    return nullptr;
    /* pthread_exit(nullptr); */
}
