/* File Name ThreadPool.cc
 * Author 泷fengyoukai
 * Github https://github.com/1750252467
 * Created Time 2023-03-04 20:26
 * Last Modified 2023-03-04 21:02
 */
#include "ThreadPool.h"
#include "WorkThread.h"
#include <unistd.h>

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
:_threadNum(threadNum)
,_queSize(queSize)
,_taskQue(queSize)
,_isExit(false)
{
    _threads.reserve(threadNum);

}

ThreadPool::~ThreadPool()
{
    if(!_isExit)
    {
        stop();
        _isExit = true;
    }
}

void ThreadPool::start()
{
    for(size_t idx = 0; idx < _threadNum; ++idx)
    {
        unique_ptr<Thread> up(new WorkThread(*this));
        _threads.push_back(std::move(up));
    }

    for(auto &th : _threads)
    {
        th->start();
    }
}

void ThreadPool::stop()
{
    while(!_taskQue.empty())
    {
        sleep(1);
    }

    _isExit = true;
    _taskQue.wakeup();

    for(auto &th : _threads)
    {
        th->join();
    }

}


void ThreadPool::addTask(Task *ptask)
{
    if(ptask)
    {
        _taskQue.push(ptask);
    }

}
Task *ThreadPool::getTask()
{
    return _taskQue.pop();
}


//线程交给工作线程做的任务
void ThreadPool::threadFunc()
{
    //线程池只要不退出，任务只要没有执行完，我们的工作线程
    //是需要一直获取任务，并且执行任务
    while(!_isExit)
    {
        //先获取任务
        Task *ptask = getTask();
        if(ptask)
        {
            ptask->process();//工作线程做的任务
        }
    }
}














