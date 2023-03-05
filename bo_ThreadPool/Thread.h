#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include <functional>

using std::function;

class Thread
{
    using ThreadCallback = function<void()>;
public:
    /* Thread(const ThreadCallback &cb); */
    Thread(ThreadCallback &&cb);
    ~Thread();
    void start();
    void join();
private:
    static void *threadFunc(void *arg);
private:
    pthread_t _thid;
    bool _isRunning;
    ThreadCallback _cb;

};

#endif
