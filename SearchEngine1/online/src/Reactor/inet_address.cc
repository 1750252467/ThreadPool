/*  File Name      inet_address.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 11:16
 *  Last Modified  2022-07-31 21:23
 */
#include "../../include/Reactor/inet_address.h"

#include <string.h>

namespace search_engine {

InetAddress::InetAddress(const string& ip, unsigned short port) {
    memset(&_servAddr, 0, sizeof(struct sockaddr_in));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = inet_addr(ip.c_str()); // host->network
    _servAddr.sin_port = htons(port);
}

InetAddress::InetAddress(const struct sockaddr_in& servAddr)
    : _servAddr(servAddr) { }

} // namespace search_engine
