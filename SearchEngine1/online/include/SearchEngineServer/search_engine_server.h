/*  File Name      search_engine_server.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 21:00
 *  Last Modified  2022-08-04 21:42
 */
#ifndef __SE_SEARCH_ENGINE_SERVER_H__
#define __SE_SEARCH_ENGINE_SERVER_H__

#include <iostream>

#include "calculate_task.h"
#include "../Reactor/tcp_server.hpp"
#include "../Reactor/thread_pool.h"
#include "../KeyRecommander/key_recommander.h"
#include "../WebPageSearcher/web_page_searcher.h"

namespace std
{
template <>
struct hash<pair<string, unsigned short>>
{
    size_t operator()(const pair<string, unsigned short> & rhs) const 
    {
            return  rhs.second * rhs.second;
    }

};

};//end of namespace std

namespace search_engine {

using MYMAP = unordered_map<std::pair<string, unsigned short>, std::pair<int, vector<string>>>;

class SearchEngineServer {
public:
    SearchEngineServer(size_t threadNum, size_t queSize,
                       const string& ip, unsigned short port);

    void start();
    void stop();

    void onMessage(const TcpConnectionPtr& con);
    void onConnection(const TcpConnectionPtr& con);
    void onClose(const TcpConnectionPtr& con);


public:
    ThreadPool        _threadPool;
    TcpServer         _tcpServer;
    KeyRecommander    _keyRecommander;
    WebPageSearcher   _webPageSearcher;  
    MYMAP             _Store;
};

} // namespace search_engine

#endif // __SE_SEARCH_ENGINE_SERVER_H__
