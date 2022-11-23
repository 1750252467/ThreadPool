/*  File Name      tcp_connection.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 21:37
 *  Last Modified  2022-08-04 23:52
 */
#ifndef __SE_TCP_CONNECTION_H__
#define __SE_TCP_CONNECTION_H__

#include <memory>
#include <functional>

#include "socket.hpp"
#include "socket_io.h"
#include "inet_address.h"

using std::shared_ptr;
using std::function;


namespace search_engine {

class EventLoop;

class TcpConnection
: public std::enable_shared_from_this<TcpConnection> {

    friend class CalculateTask;
    friend class SearchEngineServer;

    using TcpConnectionPtr      = shared_ptr<TcpConnection>;
    using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;

public:
    TcpConnection(int fd, EventLoop* loop);
    ~TcpConnection() = default;
    void send(const string& msg);
    void sendInLoop(const string& msg);
    string receive();
    string toString();


    bool isClosed() const;


    void setConnectionCallback(const TcpConnectionCallback& cb) {
        _onConectionCb = cb;
    }
    void setMessageCallback(const TcpConnectionCallback& cb) {
        _onMessageCb = cb;
    }
    void setCloseCallback(const TcpConnectionCallback& cb) {
        _onCloseCb = cb;
    }


    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    EventLoop*     _loop;
    Socket         _sock;
    SocketIO       _sockIO;
    InetAddress    _localAddr;
    InetAddress    _peerAddr;

    TcpConnectionCallback   _onConectionCb;
    TcpConnectionCallback   _onMessageCb;
    TcpConnectionCallback   _onCloseCb;
};

} // namespace search_engine

#endif // __SE_TCP_CONNECTION_H__
