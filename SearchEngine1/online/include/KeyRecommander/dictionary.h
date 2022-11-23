/*  File Name      dictionary.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-01 11:44
 *  Last Modified  2022-08-04 02:07
 */
#ifndef __SE_DICTIONARY_H__
#define __SE_DICTIONARY_H__

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

using std::string;
using std::vector;
using std::unordered_map;
using std::set;

#include "../disallow_copy_assign.h"

namespace search_engine {



class Dictionary {
public:
    static Dictionary* getInstance();
    void destroy();
    
    vector<std::pair<string, int>> doQuery(const string& key);

    /* void queryIndex(); */

    //1. 求取一个字符占据的字节数
    size_t nBytesCode(const char ch);
    //2. 求取一个字符串的字符长度
    std::size_t length(const std::string &str);
    //3. 中英文通用的最小编辑距离算法
    int editDistance(const std::string & lhs, const std::string &rhs);
    //4.
    int triple_min(const int& a, const int& b, const int& c);
    //实现c++中英文字符串按块切分
    void handleWord(const string& input, vector<string>& words);


    void loadDict();
    void loadIndex();
    void loadIdMap();

private:
    DISALLOW_COPY_AND_ASSIGN(Dictionary);
    Dictionary() = default; 

private:
    static Dictionary*   _pdict;
        // 词典 :  单词 + 所有文章中频率
    unordered_map<string , int>            _dict; 

       // 索引  :  字 + 单词id集合
    unordered_map<string, set<int>>        _index;

      // id映射 :  单词id + 单词
    unordered_map<int, string>             _idMap;
};


} // namespace search_engine

#endif // __SE_DICTIONARY_H__
