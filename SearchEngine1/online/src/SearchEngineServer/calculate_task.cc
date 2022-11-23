/*  File Name      calculate_task.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-01 17:13
 *  Last Modified  2022-08-05 14:22
 */
#include "../../include/SearchEngineServer/calculate_task.h"
#include "../../include/protocol_parser.hpp"
#include "../../include/LRUCache/lru_cache.h"
#include "../../include/LRUCache/cache_manager.h"

#include "../../include/SearchEngineServer/search_engine_server.h"
#include "../../include/Reactor/tcp_connection.h"
#include "../../include/Reactor/inet_address.h"

#include "../../include/my_log.h"

namespace search_engine {

extern __thread const char* name;

void CalculateTask::wordSearcher() {
     LRUCache lru = CacheManager::getInstance()->getCache(string(name));
     
     vector<string> tmp;
     int ret = lru.get(_key._sought, tmp);

     if (0 == ret) {
         //找到了
         string res;
         res = "[关键词推荐][cache]\n";
        /* _con->sendInLoop("关键词推荐[cache]\n"); */
        int cnt = 1;
        for(auto& val : tmp) {
            res += std::to_string(cnt) + "." + val + " ";
            ++cnt;
            /* res += " "; */
           /* _con->sendInLoop(val + " "); */
        }
        res += "\n";
        _con->sendInLoop(res);
        LogInfo("[LogInfo]word query");
        /* _con->sendInLoop("\n"); */
        //1.写回自己的cache块
        CacheManager::getInstance()->getCache(string(name)).put
            (_key._sought, tmp);
        //2.再加到自己的pendinglist上，等定时器通知主缓存去更新同步
        CacheManager::getInstance()->getCache(string(name)).addRecord
            (_key._sought, tmp);
     } else {
         //没找到，则查询，查询后更新对应

         vector<string> finals = _key.doQuery();
        if(!finals.size()) {
            _con->sendInLoop("未找到适配关键词，请重新输入。\n");
            return;
        } else {
            string res = "[关键词推荐]\n";
            /* _con->sendInLoop("关键词推荐\n"); */
            int cnt = 1;
            for(auto& val : finals) {
                res += std::to_string(cnt) + "." + val + " ";
                ++cnt;
               /* _con->sendInLoop(val + " "); */
            }
            res += "\n";
            _con->sendInLoop(res);
            LogInfo("[LogInfo]word query");
            /* _con->sendInLoop("\n"); */
            //1.写回自己的cache块
        CacheManager::getInstance()->getCache(string(name)).put
            (_key._sought, finals);
            //2.再加到自己的pendinglist上，等定时器通知主缓存去更新同步
        CacheManager::getInstance()->getCache(string(name)).addRecord
            (_key._sought, finals);
        }
     }
     
}

void CalculateTask::pageSearcher() {
   string longLine = "<-------------------------------------------------------------->\n";
     LRUCache lru = CacheManager::getInstance()->getCache2(string(name));
     
     vector<string> tmp;
     int ret = lru.get(_web._sought, tmp);
     if(0 == ret) {
         //缓存有
         string send = tmp[0] + "[cache]\n";
        _con->sendInLoop(send);
        LogInfo("[LogInfo]page query");
        //1.写回自己的cache块
        CacheManager::getInstance()->getCache2(string(name)).put
            (_web._sought, tmp);
        //2.再加到自己的pendinglist上，等定时器通知主缓存去更新同步
        CacheManager::getInstance()->getCache2(string(name)).addRecord
            (_web._sought, tmp);
     } else {
         //缓存没有
        vector<string> finals = _web.doQuery();
        if(finals.size() == 0) {
            _con->sendInLoop("未找到适配文章，请重新输入。\n");
            return;
        } else {
            InetAddress addr = _con->getPeerAddr();
            string ip = addr.ip();
            unsigned port = addr.port();
            string Line; 

            if(finals.size() > 3) {
                size_t i = 0;
                for(; i < finals.size(); ++i) {
                    if(i < 3) {
                        Line += finals[i];
                    }else{
                        break;
                    }
                }
                _Store[std::make_pair(ip, port)] = std::make_pair(i, finals);

            }else {
                for(auto word : finals) {
                    Line += word;
                }
                _Store[std::make_pair(ip, port)] = std::make_pair(-1, finals);

            }
            Line += longLine;
            _con->sendInLoop(Line);
            LogInfo("[LogInfo]page query");
            vector<string> tmp;
            tmp.push_back(Line);
            //1.写回自己的cache块
        CacheManager::getInstance()->getCache2(string(name)).put
            (_web._sought, tmp);
            //2.再加到自己的pendinglist上，等定时器通知主缓存去更新同步
        CacheManager::getInstance()->getCache2(string(name)).addRecord
            (_web._sought, tmp);
        }
     }
}

void CalculateTask::nextPageSearcher() {

    string longLine = "<-------------------------------------------------------------->\n";
    string Line;
    InetAddress addr = _con->getPeerAddr();
    string ip = addr.ip();
    unsigned port = addr.port();
    auto p = std::make_pair(ip, port);
    auto it = _Store[p];
    if(it.first == -1) {
        _con->sendInLoop("当前网页搜索暂无其他内容\n");
    }else {
        size_t cur = it.first ;
        auto vc = it.second;
        for(size_t i = 0; i < 3; ++i) {
            if(++cur >= vc.size()) {
                cur %= vc.size();
            }
            Line += vc[cur];
        }
        Line += longLine;
        _con->sendInLoop(Line);

        _Store[std::make_pair(ip, port)] = std::make_pair(cur, vc);
    }
}


void CalculateTask::process() {
    ProtocolParser parser(_msg);


    set<int > cmdSet = {1, 2, 3, 4, 5, 6};
    /* set<int > switchPageCmdSet = {3, 4, 5, 6}; */
    if ( cmdSet.end() == cmdSet.find(parser._id)) {
    string send = "错误命令，请输入：\n\
\"1 查询内容\" 请求推荐关键字\n\
\"2 查询内容\" 查询网页\n\
\"3\"          往下翻页\n\
\"4\"          往上翻页\n\
\"5\"          回到首页\n\
\"6\"          去往尾页\n";
    _con->sendInLoop(send);
    } else {
        if (1 == parser._id) {
            _key.set_kr_value(parser._sought, _con);
            wordSearcher();
        } else if (2 == parser._id) {
            _web.set_m_val(parser._sought, _con);
            pageSearcher();
        } else {  // 3, 4, 5, 6  // 可以写在一个switchPage函数里
            InetAddress addr = _con->getPeerAddr();
            string ip = addr.ip();
            unsigned port = addr.port();
            auto p = std::make_pair(ip, port);
            auto it = _Store[p];  // 
            if (0 == it.second.size()) {
                _con->sendInLoop("当前网页搜索暂无其他内容\n");
            } else {
                if(it.first == -1) {
                    _con->sendInLoop("当前网页搜索暂无其他内容\n");
                } else {
                    string longLine = "<-------------------------------------------------------------->\n";
                    string Line;
                    size_t cur = it.first ;
                    auto vc = it.second;   

                    if (3 == parser._id) { // forward
                        for(size_t i = 0; i < 3; ++i) {
                            if(cur >= vc.size()) { cur %= vc.size(); }
                            Line += vc[cur];
                            ++cur; 
                        }
                    } else if (4 == parser._id) { // backward
                        for(size_t i = 0; i < 6; ++i) {
                            if(--cur == -1) { cur = vc.size() - 1; }
                        }
                        for(size_t i = 0; i < 3; ++i) {
                            if(cur >= vc.size()) { cur %= vc.size(); }
                         Line += vc[cur];
                            ++cur; 
                        }
                    } else if (5 == parser._id) { // first
                        cur = 0;
                        for (size_t i = 0; i < 3; ++i) {
                            Line += vc[cur++];
                        }
                    } else if (6 == parser._id) { // last
                        cur = vc.size() - 3;
                        for (size_t i = 0; i < 3; ++i) {
                            Line += vc[cur++];
                    }
                        cur = 0; // 回到首页
                    }
                    Line += longLine;
                    _con->sendInLoop(Line);
                    _Store[std::make_pair(ip, port)] = std::make_pair(cur, vc); // 更新i为cur
                }

            }
        }
    }
}







/* void CalculateTask::process() { */
/*     ProtocolParser parser(_msg); */

/*     if (1 == parser._id) { */
/*         _key.set_kr_value(parser._sought, _con); */
/*         wordSearcher(); */
/*     } else if (2 == parser._id) { */
/*         _web.set_m_val(parser._sought, _con); */
/*         pageSearcher(); */
/*     } else if (3 == parser._id){ */
/*         nextPageSearcher(); */
/*     } else { */
/*         _con->sendInLoop("错误命令，请输入：\n\"1 查询内容\" 请求推荐关键字\n\"2 查询内容\" 查询网页\n"); */
/*     } */
/* } */



} // namespace search_engine
