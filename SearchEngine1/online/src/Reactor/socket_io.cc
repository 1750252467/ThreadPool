/*  File Name      socket_io.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 21:26
 *  Last Modified  2022-07-31 21:24
 */
#include "../../include/Reactor/socket_io.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

namespace search_engine {

int SocketIO::readN(char* buf, int len) {
    int left = len;
    char* pstr = buf;
    int ret = 0;

    while (left > 0) {
        ret = read(_fd, pstr, left);
        if (-1 == ret && errno == EINTR) {
            continue;
        } else if (-1 == ret){
            perror("read error -1");
            return len - ret;
        } else if (0 == ret){
            break;
        } else {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;
}

int SocketIO::readLine(char* buf, int len) {
    int left = len - 1;
    char* pstr = buf;
    int ret = 0, total = 0;

    while (left > 0) {
        //MSG_PEEK不会将缓冲区中的数据进行清空,只会进行拷贝操作
        ret = recv(_fd, pstr, left, MSG_PEEK);
        if (-1 == ret && errno == EINTR) {
            continue;
        } else if (-1 == ret) {
            perror("readLine error -1");
            return len - ret;
        } else if (0 == ret) {
            break;
        } else {
            for (int idx = 0; idx < ret; ++idx) {
                if (pstr[idx] == '\n') {
                    int sz = idx + 1;
                    readN(pstr, sz);
                    pstr += sz;
                    *pstr = '\0';

                    return total + sz;
                }
            }
            readN(pstr, ret);//从内核态拷贝到用户态
            total += ret;
            pstr += ret;
            left -= ret;
        }
    }
    *pstr = '\0';

    return total - left;
}

int SocketIO::writeN(const char* buf, int len) {
    int left = len;
    const char* pstr = buf;
    int ret = 0;

    while (left > 0) {
        ret = write(_fd, pstr, left);
        if(-1 == ret && errno == EINTR) {
            continue;
        } else if (-1 == ret) {
            perror("writen error -1");
            return len - ret;
        } else if (0 == ret) {
            break;
        } else {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;
}

} // namespace search_engine
