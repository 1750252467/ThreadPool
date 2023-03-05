#include "ThreadPool.h"
#include <unistd.h>

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(queSize) 
, _isExit(false)
{
    //预留空间
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

//命令模式、ctrl + v、方向键、shift + i、添加文字、esc

void ThreadPool::start()
{
    //需要将子线程存放在vector
    for(size_t idx = 0; idx < _threadNum; ++idx)
    {
        unique_ptr<Thread> up(new Thread(std::bind(&ThreadPool::threadFunc, this)));
        _threads.push_back(std::move(up));
    }

    //将vector的子线程跑起来
    for(auto &th : _threads)
    {
        th->start();//工作线程运行起来
    }
}

void ThreadPool::stop()
{
    //只要任务队列中存在数据，子线程就不能退出
    while(!_taskQue.empty())
    {
        sleep(1);
    }

    _isExit = true;
    _taskQue.wakeup();

    for(auto &th : _threads)
    {
        th->join();//回收工作线程
    }


}

void ThreadPool::addTask(Task &&task)
{
    if(task)
    {
        _taskQue.push(std::move(task));
    }
}

Task ThreadPool::getTask()
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
        Task taskcb = getTask();
        if(taskcb)
        {
            taskcb();//回调函数
        }
    }
}
