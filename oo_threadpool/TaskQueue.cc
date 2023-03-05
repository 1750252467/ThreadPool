/* File Name TaskQueue.cc
 * Author 泷fengyoukai
 * Github https://github.com/1750252467
 * Created Time 2023-03-04 18:49
 * Last Modified 2023-03-04 19:26
 */
#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t queueSize)
: _queSize(queueSize)
,_que()
,_mutex()
,_notFull(_mutex)
,_notEmpty(_mutex)
,_flag(true)
{

}

TaskQueue::~TaskQueue()
{

}

bool TaskQueue::empty() const
{
    return 0 == _que.size();
}

bool TaskQueue::full() const
{
    return _que.size() == _queSize;
}

void TaskQueue::push(ElemType value)
{
    MutexLockGuard autoLock(_mutex);//栈对象 RAII技术，防止业务中间条件导致无法释放锁
    while(full())
    {
        _notFull.wait();
    }

    _que.push(value);

    _notEmpty.notify();
}

ElemType TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);
    while(_flag && empty())
    {
        //上半部：排队，释放锁，睡眠
        //下半部：被唤醒，拿到锁，返回
        _notEmpty.wait();
    }

    if(_flag)
    {
        ElemType tmp = _que.front();
        _que.pop();

        _notFull.notify();

        return tmp;
    }
    else
    {
        return nullptr;
    }
}


void TaskQueue::wakeup()
{
    _flag = false;
    _notEmpty.notifyAll();
}
