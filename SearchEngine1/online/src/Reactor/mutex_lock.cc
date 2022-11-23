/*  File Name      mutex_lock.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 11:49
 *  Last Modified  2022-07-31 21:23
 */
#include "../../include/Reactor/mutex_lock.h"

#include <stdio.h>

namespace search_engine {

MutexLock::MutexLock() {
    int ret = pthread_mutex_init(&_mutex, nullptr);
    if (ret) {
        perror("pthread_mutex_init");
    }
}

MutexLock::~MutexLock() {
    int ret = pthread_mutex_destroy(&_mutex);
    if (ret) {
        perror("pthread_mutex_destroy");
    }
}

void MutexLock::lock() {
    int ret = pthread_mutex_lock(&_mutex);
    if (ret) {
        perror("pthread_mutex_lock");
    }
}

void MutexLock::unlock() {
    int ret = pthread_mutex_unlock(&_mutex);
    if(ret) {
        perror("pthread_mutex_unlock");
    }
}

} // namespace search_engine
