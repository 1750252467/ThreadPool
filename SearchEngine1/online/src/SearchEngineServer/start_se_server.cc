/*  File Name      start_se_server.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 22:27
 *  Last Modified  2022-08-05 11:25
 */
#include "../../include/configuration.h"
#include "../../include/KeyRecommander/dictionary.h"
#include "../../include/WebPageSearcher/web_page_query.h"
#include "../../include/SearchEngineServer/search_engine_server.h"
#include "../../include/LRUCache/cache_manager.h"
#include "../../include/TimerManager/timer_fd.h"
#include "../../include/TimerManager/cache_thread.h"

using std::string;

using namespace search_engine;


int main() {

    map<string, string> configs = 
    Configuration::getInstance()->get_configs();

    int threadNum = stoi(configs["threadNum"]);
    int queSize   = stoi(configs["queSize"]);
    string ip     = configs["ip"];
    size_t port   = stoi(configs["port"]);

    Dictionary::getInstance();
    web_page_query::get_Web_Query_ptr();

    cout << "loading server ..." << endl;

    CacheManager *p= CacheManager::getInstance();
    TimerFd tf1(2, 8, std::bind(&CacheManager::updateCache, p));
    CacheThread th1(bind(&TimerFd::start, &tf1), std::to_string(0));
    th1.start();
    TimerFd tf2(3, 15, std::bind(&CacheManager::updateCache2, p));
    CacheThread th2(bind(&TimerFd::start, &tf2), std::to_string(0));
    th2.start2();

    SearchEngineServer seServer(threadNum, queSize, ip, port);
    seServer.start();
    return 0;
}
