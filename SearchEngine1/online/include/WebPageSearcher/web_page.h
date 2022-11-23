#ifndef __SE_WEBPAGE_H__
#define __SE_WEBPAGE_H__


#include "cppjieba/Simhasher.hpp"
#include <string>
#include <vector>
#include <map>
#include <set>

using std::set;
using std::map;
using std::vector;
using std::string;

namespace search_engine
{

class Configuration;
class WordSegmentation;

class webPage
{
public:
    webPage();
    webPage(string & doc, WordSegmentation * jieba, simhash::Simhasher * sim);
    ~webPage();
    int getDocid();
    string getTile();
    string getUrl();
    string summary(const vector<string> & quertWord);
    map<string, int> & getWordsMap();
private:
    void processDoc(const string & doc); 
    void cala_TopK(vector<string> & wordsVec, int k); 

private:
    const static int TOP_NUM = 20;
    int     _docid;
    string  _docTitle;
    string  _docUrl;
    string  _docContent;
    string  _docSummary;
    vector<string>  _topWords;
    map<string, int>     _wordsMap;
    WordSegmentation *   _jieba;
    simhash::Simhasher *  _sim;

};

};// end of namespace search_engine
#endif // end of __SE_WEBPAGE_H__

