/*  File Name      cache_manager.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-03 10:23
 *  Last Modified  2022-08-05 10:19
 */
#include "../../include/LRUCache/cache_manager.h"


namespace search_engine {
    CacheManager*  CacheManager:: pInstance = nullptr;
    extern __thread const char*name2;

CacheManager* CacheManager::getInstance() {
    if(!pInstance) {
       pInstance = new CacheManager();
    }
    return pInstance;
}

CacheManager::CacheManager() {
        init("data/cache.dat");
}

void CacheManager::init(const string& filename) {
        _caches.reserve(7);
        _caches2.reserve(7);
        for(int i = 0; i < 7; i++) {
            _caches.emplace_back(LRUCache(5));
            _caches2.emplace_back(LRUCache(3));
        }
}
    
void CacheManager::updateCache() {
     printf("\e[0;35m" "wordCache thread[%s] got a task[update cache]\n" "\e[0m", name2);

     //把所有子cache的pendinglist的node依次插入主cache中
     //然后情况pendingList
     int cnt = 0;
     for(int i = 1; i < 7; ++i) {
         if (!_caches[i]._pendingUpdateList.size()) {
             ++cnt;
             continue;
         }
        _caches[0].mergePending(_caches[i]);
        _caches[i]._pendingUpdateList.clear();
     }
     
     if (6 == cnt) {
         printf("\e[0;35m" "wordCache thread[%s] finished a task[no data to update]\n" "\e[0m", name2);
         return;
     }

     for(int i = 1; i < 7; ++i) {
         _caches[i].update(_caches[0]);
     }
     printf("\e[0;35m" "wordCache thread[%s] finished a task[update cache success]\n" "\e[0m", name2);
}

void CacheManager::updateCache2() {
     printf("\e[0;35m" "pageCache thread[%s] got a task[update cache]\n" "\e[0m", name2);

     //把所有子cache的pendinglist的node依次插入主cache中
     //然后情况pendingList
     int cnt = 0;
     for(int i = 1; i < 7; ++i) {
         if (!_caches2[i]._pendingUpdateList.size()) {
             ++cnt;
             continue;
         }
        _caches2[0].mergePending(_caches2[i]);
        _caches2[i]._pendingUpdateList.clear();
     }

     if (6 == cnt) {
         printf("\e[0;35m" "pageCache thread[%s] finished a task[no data to update]\n" "\e[0m", name2);
         return;
     }

     for(int i = 1; i < 7; ++i) {
         _caches2[i].update(_caches2[0]);
     }
     printf("\e[0;35m" "pageCache thread[%s] finished a task[update cache success]\n" "\e[0m", name2);
}

LRUCache& CacheManager::getCache(string name) {
    int idx = stoi(name);
    return _caches[idx];
}
    
LRUCache& CacheManager::getCache2(string name) {
    int idx = stoi(name);
    return _caches2[idx];
}



}// namespace search_engine

