/* File Name ThreadPool.h
 * Author 泷fengyoukai
 * Github https://github.com/1750252467
 * Created Time 2023-03-04 20:04
 * Last Modified 2023-03-04 20:45
 */
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Thread.h"
#include "TaskQueue.h"

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class ThreadPool
{   //为了访问ThreadPool中的threadFunc
    friend class WorkThread;
public:
    ThreadPool(size_t threadNum, size_t queueSize);
    ~ThreadPool();

    void start();
    void stop();

    void addTask(Task *ptask);
private:
    Task * getTask();//获取任务
    void threadFunc();//线程池交给工作线程执行的任务

    size_t _threadNum;//子线程的数目
    size_t _queSize;//任务队列的大小
    vector<unique_ptr<Thread>> _threads;//存子线程的容器
    TaskQueue _taskQue;// 任务队列对象
    bool _isExit;//线程池是否退出的标志



};


#endif // __THREADPOOL_H__
