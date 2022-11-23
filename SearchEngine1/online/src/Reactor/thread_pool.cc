/*  File Name      thread_pool.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 22:21
 *  Last Modified  2022-08-04 16:17
 */
#include "../../include/Reactor/thread_pool.h"
/* #include "../../include/LRUCache/cache_manager.h" */
/* #include "../../include/TimerManager/timer_fd.h" */
/* #include "../../include/TimerManager/cache_thread.h" */

#include "unistd.h"

namespace search_engine {

extern __thread const char* name;

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum),
  _queSize(queSize),
  _taskQue(_queSize),
  _isExit(false) {
    _threads.reserve(_threadNum);
}

ThreadPool::~ThreadPool() {
    if (!_isExit) {
        stop();
        _isExit = true;
    }
}

void ThreadPool::start() {
    //创建工作线程并存储到vector
    for (size_t idx = 1; idx < _threadNum - 1; ++idx) {
         _threads.emplace_back(new Thread(bind(&ThreadPool::threadFunc, this),
                std::to_string(idx)));
    }


    for (unique_ptr<Thread>& th : _threads) {
        th->start();
    }
}

void ThreadPool::stop() {
    // 任务队列中只要有数据，就一定要执行完毕，不能让子线程退出
    while (!_taskQue.empty()) {
        sleep(0);
    }

    _isExit = true;         //置标志位
    _taskQue.wakeup();      //所有的工作线程都唤醒

    for (auto& th : _threads) {
        th->join();         //工作线程的退出
    }
}

void ThreadPool::addTask(Task&& task) {
    if (task) {
        _taskQue.push(std::move(task));
    }
}

Task ThreadPool::getTask() {
    return _taskQue.pop();
}


void ThreadPool::threadFunc() {
    while (!_isExit) {
        Task calculateTaskcb = getTask();

        if (calculateTaskcb) {
            string out = "calculate thread[" + string(name) + "] got a task";
            printf("\e[0;32m" "%s\n" "\e[0m", out.c_str());

            calculateTaskcb();    
        }
    }
}

} // namespace search_engine
