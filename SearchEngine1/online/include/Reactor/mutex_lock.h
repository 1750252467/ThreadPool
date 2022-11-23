/*  File Name      mutex_lock.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 14:04
 *  Last Modified  2022-07-31 23:39
 */
#ifndef __SE_MUTEX_LOCK_H__
#define __SE_MUTEX_LOCK_H__

#include <pthread.h>

#include "../disallow_copy_assign.h"

namespace search_engine {

class MutexLock {
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    pthread_mutex_t* get_mutex_lock_ptr()   { return & _mutex; }

private:
    DISALLOW_COPY_AND_ASSIGN(MutexLock);

    pthread_mutex_t _mutex;
};


class MutexLockGuard {
public:
    MutexLockGuard(MutexLock &mutex)
    : _mutex(mutex)   {
        _mutex.lock();
    }

    ~MutexLockGuard() { _mutex.unlock(); }

private:
    MutexLock& _mutex;
};

} // namespace search_engine

#endif // __SE_MUTEX_LOCK_H__
