/*  File Name      cache_thread.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-04 03:38
 *  Last Modified  2022-08-04 22:52
 */
#ifndef __SE_CACHE_THREAD_H__
#define __SE_CACHE_THREAD_H__
#include <pthread.h>
#include <functional>
#include <string>

using std::function;
using std::string;

namespace search_engine {

using ThreadCallback = function<void()>;

class CacheThread {
public:
    /* Thread(const ThreadCallback &cb); */
    CacheThread(ThreadCallback &&cb, const string&name = string());
    virtual ~CacheThread();

    void start() {
        //threadFunc要设置为静态函数，原因是pthread_create函数第三个参数
        //是一个函数指针，该指针的类型已经被严格固定了，而如果将threadFunc
        //设计为成员函数的时候，会多一个this，所以需要设置为静态的
        //this  --- > pth  ---- > new MyThread
        int ret = pthread_create(&_thid, nullptr, threadFunc, this);
        if(ret)
        {
            perror("pthread_create");
            return;
        }
        _isRunning = true;
        printf("\e[0;35m" "wordCache thread[%s] created\n" "\e[0m", _name.c_str());
    }

    void start2() {
        //threadFunc要设置为静态函数，原因是pthread_create函数第三个参数
        //是一个函数指针，该指针的类型已经被严格固定了，而如果将threadFunc
        //设计为成员函数的时候，会多一个this，所以需要设置为静态的
        //this  --- > pth  ---- > new MyThread
        int ret = pthread_create(&_thid, nullptr, threadFunc, this);
        if(ret)
        {
            perror("pthread_create");
            return;
        }
        _isRunning = true;
        printf("\e[0;35m" "pageCache thread[%s] created\n" "\e[0m", _name.c_str());
    }

    void join();
private:
    static void *threadFunc(void *arg);

private:
    pthread_t _thid;
    string    _name;
    bool _isRunning;
    ThreadCallback _cb;
};

} // namespace search_engine

#endif // __SE_CACHE_THREAD_H__
