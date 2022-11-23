/*  File Name      thread.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 22:09
 *  Last Modified  2022-08-04 11:25
 */
#ifndef __SE_THREAD_H__
#define __SE_THREAD_H__

#include <pthread.h>

#include <string>
#include <iostream>

#include <functional>


using std::function;
using std::string;

namespace search_engine {

extern __thread const char *name;

using ThreadCallback = function<void()>;

class Thread {
public:
    Thread(ThreadCallback&& cb, const string&name = string());

    virtual ~Thread() {
        if(_isRunning) {
            pthread_detach(_thid);
        }
    }

    void start() {
        //threadFunc要设置为静态函数，原因是pthread_create函数第三个参数
        //是一个函数指针，该指针的类型已经被严格固定了，而如果将threadFunc
        //设计为成员函数的时候，会多一个this，所以需要设置为静态的
        //this  --- > pth  ---- > new MyThread
        int ret = pthread_create(&_thid, nullptr, threadFunc, this);
        if (ret) {
            perror("pthread_create");
            return;
        }
        _isRunning = true;
        printf("\e[0;32m" "calculate thread[%s] created\n" "\e[0m", _name.c_str());
    }

    void join() {
        if (_isRunning) {
            pthread_join(_thid, nullptr);
            _isRunning = false;
        }
    }

private:
    static void* threadFunc(void* arg);

private:
    pthread_t        _thid;
    string           _name;
    bool             _isRunning;
    ThreadCallback   _cbPool;
};

} // namespace search_engine

#endif // __SE_THREAD_H__
