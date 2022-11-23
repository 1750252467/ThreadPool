/*  File Name      acceptor.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-28 19:59
 *  Last Modified  2022-07-31 21:22
 */
#include "../../include/Reactor/acceptor.h"

#define MAX_LISTEN_LIST 16

namespace search_engine{

Acceptor::Acceptor(const string& ip, unsigned short port)
: _listenSock(),
  _servAddr(ip, port) { }

void Acceptor::ready() {
    setReuse();
    bind();
    listen();
}

void Acceptor::setReuse() {
    int reuse = 1;
    int ret = 0;
    ret = setsockopt(_listenSock.fd(), 
                     SOL_SOCKET, 
                     SO_REUSEADDR, 
                     &reuse,  sizeof(reuse));
    if (-1 == ret) {
        perror("setsockopt ADDR");
        return;
    }
    ret = setsockopt(_listenSock.fd(), 
                     SOL_SOCKET,
                     SO_REUSEPORT, 
                     &reuse, sizeof(reuse));
    if (-1 == ret) {
        perror("setsockopt PORT");
        return;
    }
}

void Acceptor::bind() {
    int ret = ::bind(_listenSock.fd(), 
                     (struct sockaddr*)_servAddr.get_inet_address_ptr(), 
                     sizeof(_servAddr));
    if (-1 == ret) {
        perror("bind");
        return;
    }
}

void Acceptor::listen() {
    int ret = ::listen(_listenSock.fd(), MAX_LISTEN_LIST);
    if (-1 == ret) {
        perror("listen");
        return;
    }
}

} // namespace search_engine
