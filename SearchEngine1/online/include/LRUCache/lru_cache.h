/*  File Name      lru_cache.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-02 20:15
 *  Last Modified  2022-08-04 17:48
 */
#ifndef __SE_LRUCACHE_H__
#define __SE_LRUCACHE_H__

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

using std::string;
using std::list;
using std::unordered_map;
using std::vector;

namespace search_engine {

class LRUCache {
    friend class CacheManager;
public:
    LRUCache(int capacity)
        : _capacity(capacity) { }

    int  get(string key, vector<string>& val);

    void put(string key, vector<string> value);

    void addRecord(string key, vector<string> val);

    void mergePending(const LRUCache& lrucache);

    void update(const LRUCache& lrucache);

private:
    struct CacheNode {
        CacheNode(string k, vector<string> v)
            : key(k), 
              value(v) { }

        string key;
        vector<string> value;
    };

private:
    unordered_map<string, list<CacheNode>>::size_type   _capacity;
    list<CacheNode>                                     _nodes;
    unordered_map<string, list<CacheNode>::iterator>    _cache;
    list<CacheNode>   _pendingUpdateList;
};
} // namespace search_engine

#endif // __SE_LRUCACHE_H__
