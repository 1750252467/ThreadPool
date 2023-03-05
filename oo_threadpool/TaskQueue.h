/* File Name TaskQueue.h
 * Author 泷fengyoukai
 * Github https://github.com/1750252467
 * Created Time 2023-03-04 18:29
 * Last Modified 2023-03-04 21:06
 */
#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"
#include "Task.h"
#include <queue>

using std::queue;

using ElemType = Task *;

class TaskQueue{
public:
    TaskQueue(size_t queSize);
    ~TaskQueue();
    bool empty() const;
    bool full() const;
    void push(ElemType value);
    ElemType pop();

    void wakeup();

private:
    size_t _queSize;
    queue<ElemType> _que;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    bool _flag;

};


#endif // __TASKQUEUE_H__
