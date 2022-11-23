/*  File Name      tcp_server.hpp
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 21:57
 *  Last Modified  2022-08-01 20:52
 */
#ifndef __SE_TCP_SERVER_H__
#define __SE_TCP_SERVER_H__

#include "acceptor.h"
#include "event_loop.h"

namespace search_engine {

class TcpServer {
public:
    TcpServer(const string &ip, unsigned short port)
    : _acceptor(ip, port),
      _loop(_acceptor)       {    }

    void start() {
        _acceptor.ready();
        _loop.loop();
    }

    void stop() {
        _loop.unloop();
    }

    void setAllCallback(TcpConnectionCallback &&onConnection,
                        TcpConnectionCallback &&onMessage,
                        TcpConnectionCallback &&onClose)    {
        _loop.setConnectionCallback(std::move(onConnection));
        _loop.setMessageCallback(std::move(onMessage));
        _loop.setCloseCallback(std::move(onClose));
    }

    ~TcpServer() = default;

private:
    Acceptor    _acceptor;
    EventLoop   _loop;
};

} // namespace search_engine

#endif // __SE_TCP_SERVER_H__
