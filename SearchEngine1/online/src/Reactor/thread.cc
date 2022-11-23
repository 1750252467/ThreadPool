/*  File Name      thread.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 22:01
 *  Last Modified  2022-08-03 11:21
 */
#include "../../include/Reactor/thread.h"

namespace search_engine {

__thread const char* name = "current_thread";

Thread::Thread(ThreadCallback&& cb, const string&name)
    : _thid(0),
      _name(name), 
      _isRunning(false),
      _cbPool(std::move(cb)) { }

void *Thread::threadFunc(void *arg) {
    Thread* pthread = static_cast<Thread*>(arg);
    name = pthread->_name.c_str();

    if (pthread) {
        pthread->_cbPool();
    }

    pthread_exit(nullptr);
}

} // namespace search_engine
