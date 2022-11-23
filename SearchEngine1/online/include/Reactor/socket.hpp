/*  File Name      socket.hpp
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-28 23:21
 *  Last Modified  2022-07-31 23:39
 */
#ifndef __SE_SOCKET__
#define __SE_SOCKET__

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/socket.h>

#include "../disallow_copy_assign.h"

namespace search_engine {

class Socket {
public:
    Socket() {
        _fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (_fd < 0) {
            perror("socket");
            return;
        }
    }
    Socket(int fd) 
    : _fd(fd)   {   }

    ~Socket() { close(_fd); }

    void shutDownWrite() {
        int ret = shutdown(_fd, SHUT_WR); // 关闭写端
        if (ret) {
            perror("shutdown");
            return;
        }
    }

    int fd() const { return _fd; }

private:
    DISALLOW_COPY_AND_ASSIGN(Socket);

    int _fd;
};

} // namespace search_engine

#endif // __SE_SOCKET__
