#ifndef __SE_WEB_PAGE_QUERY_H__
#define __SE_WEB_PAGE_QUERY_H__

#include "cppjieba/Simhasher.hpp"
#include "word_segmentation.h"
#include "web_page.h"

#include <unordered_map>
#include <set>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::set;
using std::unordered_map;

namespace search_engine
{


class web_page_query
{
public:
    static web_page_query * get_Web_Query_ptr() {
        if(pQuery == nullptr) {
            pQuery = new web_page_query();
            atexit(destroy);
        }
        return pQuery;
    }
    static void destroy() {
        if(pQuery) {
            delete pQuery;
        }
    }
    vector<string> doQuery(const string & str);
private:
    web_page_query();
    ~web_page_query();
private:
    void loadLibary();
    vector<double> get_Query_WordsWeight_Vector(std::vector<string> & queryWords);
    bool executeQuery(const vector<string> & queryWords,
                      unordered_map<int, vector<double>> & resultVec);
    vector<string> create_Json(vector<int> & docidVec, const vector<string> & queryWords);
    string return_NoAnswer();

    static bool ComPair(const std::pair<int, double> & lhs, const std::pair<int, double> & rhs) {
        return lhs.second > rhs.second;
    }
private:
    static web_page_query * pQuery;
    WordSegmentation _jieba;
    simhash::Simhasher  _sim;
    unordered_map<int, webPage> _pageLib;
    unordered_map<int, std::pair<int, int>> _offsetLib;
    unordered_map<string, set<std::pair<int, double>>> _inverIndexTable;
};



};//end of namespace search_engine

#endif  //end of __SE_WEB_PAGE_QUERY_H__
