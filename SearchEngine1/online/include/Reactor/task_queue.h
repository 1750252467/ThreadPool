/*  File Name      task_queue.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 20:10
 *  Last Modified  2022-07-29 23:30
 */
#ifndef __SE_TASK_QUEUE_H__
#define __SE_TASK_QUEUE_H__

#include <queue>
#include <functional>

#include "mutex_lock.h"
#include "condition.hpp"

using std::queue;
using std::function;


namespace search_engine {

using ElemType = function<void()>;

class TaskQueue {
public:
    TaskQueue(size_t queSize);
    ~TaskQueue() = default;

    bool empty() const {   return _que.size() == 0;          }
        
    bool full()  const {   return _que.size() == _queSize;   }

    /* void push(const ElemType &value); */
    void push(ElemType&& value);
    ElemType pop();

    void wakeup() {
        _flag = false;
        _notEmpty.notifyAll();
    }


private:
    size_t            _queSize;
    queue<ElemType>   _que;
    MutexLock         _mutex;
    Condition         _notEmpty;
    Condition         _notFull;
    bool              _flag;
};

} // namespace search_engine

#endif // __SE_TASK_QUEUE_H__
