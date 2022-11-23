/*  File Name      timer_fd.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-03 16:15
 *  Last Modified  2022-08-03 16:21
 */
#ifndef __SE_TIMER_FD_H__
#define __SE_TIMER_FD_H__

#include <functional>

using std::function;

namespace search_engine {

class TimerFd {
    using TimerFdCallback = function<void()>;
public:
    TimerFd(int initsec, int perisec, TimerFdCallback &&cb);
    ~TimerFd();
    void start();
    void stop();

private:
    void handleRead();
    int createTimerFd();
    void setTimerFd(int initsec, int perisec);

private:
    int _timerfd;
    int _initsec;
    int _perisec;
    TimerFdCallback _cb;
    bool _isStarted;
};

} // namespace search_engine

#endif // __SE_TIMER_FD_H__
