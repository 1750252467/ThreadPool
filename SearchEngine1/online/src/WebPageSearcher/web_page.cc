#include "../../include/WebPageSearcher/web_page.h"

#include <string>
#include "../../include/WebPageSearcher/KMP.hpp"

#include "../../include/WebPageSearcher/word_segmentation.h"
namespace search_engine
{

webPage::webPage()
: _jieba()
, _sim()
{}

webPage::webPage(string & doc,  WordSegmentation * jieba, simhash::Simhasher * sim) 
: _jieba(jieba)
, _sim(sim)
{
    processDoc(doc);
    /* cala_TopK(_topWords, TOP_NUM); */
}
webPage::~webPage() {

}
int webPage::getDocid() {
    return _docid;
}

string webPage::getTile() {
    if(_docTitle.size()) {
        return _docTitle;
    }else{
        return nullptr;
    }
}

string webPage::getUrl() {
    if(_docUrl.size()) {
        return _docUrl;
    }else{
        return nullptr;
    }
}

string webPage::summary(const vector<string> & queryWord) {
    KMP kmp;
    set<int> Pos;
    for(auto word : queryWord) {
        Pos.insert(_docContent.find(word));
    }
    vector<int> Post;
    for(auto num : Pos) {
        Post.push_back(num);
    }

    int pos = kmp(_docContent, "。");
    
    return _docContent.substr(0, pos);
}
void webPage::processDoc(const string & doc) {
    KMP kmp;
    string doc1 = "<docid>";
    string doc2 = "</docid>";
    int dm1 = kmp(doc, doc1) + doc1.size();
    int dm2 = kmp(doc, doc2);  
    _docid = atoi(doc.substr(dm1, dm2-dm1).c_str());
    
    string t1 = "<title>";
    string t2 = "</title>";
    int tm1 = kmp(doc, t1) + t1.size();
    int tm2 = kmp(doc, t2);
    _docTitle = doc.substr(tm1, tm2 - tm1);
    
    string u1 = "<url>";
    string u2 = "</url>";
    int um1 = kmp(doc, u1) + u1.size();
    int um2 = kmp(doc, u2);
    _docUrl = doc.substr(um1, um2-um1);

    string d1 = "<description>";
    string d2 = "</description>";
    int dem1 = kmp(doc, d1) + d1.size();
    int dem2 = kmp(doc, d2);
    _docContent = doc.substr(dem1, dem2-dem1);

}
map<string, int> & webPage::getWordsMap() {
    string str = _docContent;
    vector<string> results = _jieba->operator()(str);
    for(string & word : results) {
        int & isExit = _wordsMap[word];
        if (isExit){
            ++isExit;
        } else {
            isExit = 1;
        }
    }
    return _wordsMap;
}
void webPage::cala_TopK(vector<string> & wordsVec, int k) { 
    vector<std::pair<string, double>> res;
    _sim->extract(_docContent, res, k);
    for(auto & it : res) {
        wordsVec.push_back(it.first);
    }
}





};//end of namespace search_engine
