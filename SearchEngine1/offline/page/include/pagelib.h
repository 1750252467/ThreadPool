/*  File Name      pagelib.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 22:58
 *  Last Modified  2022-07-31 22:00
 */
#ifndef __SE_PAGE_H__
#define __SE_PAGE_H__

#include "dict_producer.hpp"
#include "word_segmentation.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>

using std::string;
using std::vector;
using std::unordered_map;
using std::set;
using std::pair;
using std::map;
namespace search_engine {

class Configuration;

struct RSSIteam {
    string _title;
    string _link;
    string _description;
};
class Compare
{
    bool operator()(const pair<int,double> & lhs, const pair<int, double> & rhs) {
            return lhs.first < rhs.first;
    }
};

class PageLib{
public:
    PageLib();
    ~PageLib() {  }
    std::unordered_map<int, map<string, int>> & getWordsMap() {
        return _wordMap;
    }
    std::unordered_map<int, vector<string>> & get_TopK() {
        return _topK;
    }
private:

    void create();
    void store();
    void handle_Invert_Index();
    void handle_wordMap();
    void handle_topK();
/* private: */
    void Page_De_Duplication();
private:
    int    _DOCICNUM;
    vector<RSSIteam> _rss;
    Configuration *  _config;
    DictProducer     _dict;
    WordSegmentation & _jieba;
    /* unordered_map<int, string>   _docid; //存放 docid - docd */
    /* unordered_map<string, set<pair<int , double>, Compare>> _invertIndex; */
    unordered_map<string, set<pair<int , double>>> _invertIndex;
    std::unordered_map<int, map<string, int>> _wordMap;
    std::unordered_map<int, vector<string>>   _topK;
};

} // namespace search_engine

#endif // __SE_PAGE_H__
