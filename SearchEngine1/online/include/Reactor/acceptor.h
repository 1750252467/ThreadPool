/*  File Name      acceptor.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-28 19:54
 *  Last Modified  2022-07-29 20:48
 */
#ifndef __SE_ACCEPTOR_H__
#define __SE_ACCEPTOR_H__

#include "socket.hpp"
#include "inet_address.h"

namespace search_engine {

class Acceptor {
public:
    Acceptor(const string& ip, unsigned short port);
    ~Acceptor() = default;

    void ready();   //setReuse() -> bind() -> listen()

    int accept() {
        int connFd = ::accept(_listenSock.fd(), NULL, NULL);
        if (-1 == connFd) {
            perror("listen");
            return -1;
        }
        return connFd;
    }

    int fd() const { return _listenSock.fd(); }    


private:
    void setReuse();   // ip & port
    void bind();
    void listen();

private:
    Socket         _listenSock;
    InetAddress    _servAddr;
};

} // namespace search_engine

#endif // __SE_ACCEPTOR_H__
