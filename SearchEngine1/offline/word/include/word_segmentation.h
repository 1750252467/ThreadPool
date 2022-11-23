#ifndef __SE_WORDSEGMENTATION_H__
#define __SE_WORDSEGMENTATION_H__

#include "cppjieba/Jieba.hpp"

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace search_engine
{
const char * const DICT_PATH = "include/cppjieba/dict/jieba.dict.utf8";
//最大概率法(MPSegment: Max Probability)分词所使用的词典路径)
const char * const HMM_PATH = "include/cppjieba/dict/hmm_model.utf8";
//隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典路径
const char * const USER_DICT_PATH = "include/cppjieba/dict/user.dict.utf8";
//用户自定义词典路径
const char * const IDF_PATH = "include/cppjieba/dict/idf.utf8";
//IDF路径
 const char * const STOP_WORD_PATH = "include/cppjieba/dict/stop_words.utf8";
//停用词路径

class WordSegmentation    //使用Jieba分词库进行分词
{
public:
    WordSegmentation() : _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, 
                                IDF_PATH, STOP_WORD_PATH)
    {}
    vector<string> operator()(const string str);
    void CutSmall(const string & str, vector<string> & vec, size_t sz);
private:
    cppjieba::Jieba _jieba;
};



};//end of namespace search_engine

#endif  //__SE_WORDSEGMENTATION_H__
