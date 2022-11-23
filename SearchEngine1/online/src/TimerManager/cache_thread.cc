/*  File Name      cache_thread.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-04 03:38
 *  Last Modified  2022-08-04 11:03
 */
#include "../../include/TimerManager/cache_thread.h"
#include <stdio.h>

namespace search_engine {

__thread const char* name2 = "current_ thread";

//回调函数_cb在此处注册
CacheThread::CacheThread(ThreadCallback &&cb, const string&name)
: _thid(0)
,  _name(name)
, _isRunning(false)
, _cb(std::move(cb))//注册
{

}

CacheThread::~CacheThread()
{
    if(_isRunning)
    {
        //让子线程与主线程分离，子线程自己回收自己的资源
        pthread_detach(_thid);
    }
}


void CacheThread::join()
{
    if(_isRunning)
    {
        //shift + k可以进行系统函数的跳转
        int ret = pthread_join(_thid, nullptr);
        if(ret)
        {
            perror("pthread_join");
            return;
        }

        _isRunning = false;
    }
}

//threadFunc是一个静态的 
void *CacheThread::threadFunc(void *arg)
{
    //this如何传进来
    CacheThread *pth = static_cast<CacheThread *>(arg);
    name2 = pth->_name.c_str();
    if(pth)
    {
        //执行操作  run
        pth->_cb();//执行对应的任务,执行回调函数
    }


    /* return nullptr; */
    pthread_exit(nullptr);
}



} // namespace search_engine
