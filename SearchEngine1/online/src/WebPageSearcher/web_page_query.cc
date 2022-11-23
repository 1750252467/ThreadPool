#include "../../include/WebPageSearcher/web_page_query.h"
#include <math.h>
#include <nlohmann/json.hpp>
#include <iterator>
#include <algorithm>
#include <sstream>

using std::istringstream;
using std::ifstream;
using std::inserter;
using std::set_intersection;

namespace search_engine
{

web_page_query * web_page_query::pQuery = nullptr;

web_page_query::web_page_query()
: _jieba()
, _sim(DICT_PATH, HMM_PATH, IDF_PATH ,STOP_WORD_PATH)
{
    loadLibary();
}

web_page_query::~web_page_query()
{}

vector<string> web_page_query::doQuery(const string & str)
{
    vector<string> results = _jieba(str);
    size_t judeg = 0;
    for(auto & word : results) {
        auto it = _inverIndexTable.find(word);
        if(it == _inverIndexTable.end()) {
            ++judeg;
        }
    }
    if(judeg == results.size()) {
        return vector<string>();
    }
    vector<double> Base = get_Query_WordsWeight_Vector(results);

    unordered_map<int, vector<double>> resultVec;
    if(executeQuery(results, resultVec)) {
        //resultVec 1 (y1 y2 y3 ) 2(y1 y2 y3) ..
        double X = 0.0;
        for(double x : Base) {
            X += x * x;
        }
        X = sqrt(X);
        vector<std::pair<int, double>> web_sort;
        for(auto it = resultVec.begin(); it != resultVec.end(); ++it) {
            int docid = it->first;
            double XY = 0.0, Y = 0.0;
            for(size_t i = 0; i < it->second.size(); ++i) {
                XY += 1.0 * it->second[i] * Base[i];
                Y += 1.0 * it->second[i] * it->second[i];
            }
            Y = sqrt(Y);
            double COS = 1.0 * XY /(X * Y);
            web_sort.push_back(std::make_pair(docid, COS));
        }
        sort(web_sort.begin(), web_sort.end(), ComPair);
       
        vector<int> web_fin; //存放排序之后的网页

        for(auto it : web_sort) {
            web_fin.push_back(it.first);
        }
        
        size_t sz = web_fin.size();
        if(sz < 3){
            
        } else if(sz >= 3 && sz < 6 ){
            web_fin.resize(3);
        } else if(sz >= 6 && sz < 9) {
            web_fin.resize(6);
        } else if (sz >= 9 && sz < 12) {
            web_fin.resize(9);
        }else {
            web_fin.resize(12);
        }
        vector<string> rs = create_Json(web_fin, results);
        
        return rs;
    }else{ 
        return vector<string>();
    }
}

vector<string> web_page_query::create_Json(vector<int> & docidVec, const vector<string> & queryWords) {
    vector<string> result;
    string Line = "<-------------------------------------------------------------->\n";
    size_t id = 0;
    for(auto & docid : docidVec) {
        string res;
        string Summary = _pageLib[docid].summary(queryWords);
        string Title = _pageLib[docid].getTile();
        string Url = _pageLib[docid].getUrl();
        char buf[65536] = {0};
        sprintf(buf, "%s所查询网页:%ld\n\t标题:%s\n\t链接:%s\n\t摘要:%s。\n", 
                Line.c_str() ,++id, Title.c_str(), Url.c_str(), Summary.c_str());
        res += buf;
        result.push_back(res);
        memset(buf, 0, sizeof(buf));
    }
    return result;
}


bool web_page_query::executeQuery(const vector<string> & queryWords, unordered_map<int, vector<double>> & resultVec) {
    vector<set<int>> exist_wed;
    bool flag = false;
    for(size_t i = 0; i < queryWords.size(); ++i) {
        string word = queryWords[i];
        auto it = _inverIndexTable[word]; //set<pair<int, double>>
        set<int> tmp;
        for(auto it2 = it.begin(); it2 != it.end(); ++it2 ) {
            int docid = it2->first;
            tmp.insert(docid);
            flag = true;
        }
        exist_wed.push_back(tmp);
    }
    if(flag == false) { 
        return false;
    }
    set<int> SameWeb = exist_wed[0];
    for(size_t i = 0; i < exist_wed.size(); ++i) {
        set<int> tmp;
        set_intersection(SameWeb.begin(), SameWeb.end(), 
                         exist_wed[i].begin(), exist_wed[i].end(),
                         inserter(tmp, tmp.end()));
        SameWeb.swap(tmp);
    }
    if(!SameWeb.size()) { return false ;}
    for(auto it = SameWeb.begin(); it != SameWeb.end(); ++it) {
        int docid = *it;
        auto & it2 = resultVec[docid];
        for(string  word : queryWords) {
            auto it3 = _inverIndexTable[word];
            for(auto it4 = it3.begin(); it4 != it3.end(); ++it4) {
                if(it4->first == docid) {
                    it2.push_back(it4->second);
                    break;
                }
            }
        }
    }
    return true;
}

vector<double> web_page_query::get_Query_WordsWeight_Vector(vector<string> & queryWords) 
{
    vector<double> results; // w' w'
    unordered_map<string, int> tf;
    double N = _pageLib.size() + 1;
    for(string & word : queryWords) {
        int & isExit = tf[word];
        if(isExit) {
            ++isExit;
        }else{
            isExit = 1;
        }
    }
    vector<double> tmp; // w w w 
    for(string & word : queryWords) {
        auto it = _inverIndexTable[word];
        double DF = it.size();
        double IDF = log(N/(DF + 1)) / log(2);
        double TF = tf[word];
        double w = 1.0 * TF * IDF;
        tmp.push_back(w);
    }
    double AllW = 0.0;
    for(double w : tmp) {
        AllW += w * w;
    }
    AllW = sqrt(AllW);
    for(double w : tmp) {
        double FinW = w / AllW;
        results.push_back(FinW);
    }
    return results;
}
void web_page_query::loadLibary() {
    ifstream ifs_off;
    ifs_off.open("../offline/page/data/offset.dat");
    cout << "loading offset.dat ..." << endl;
    string line;
    while(ifs_off >> line) {
        int id = atoi(line.c_str());
        int begin;
        int end;
        ifs_off >> begin;
        ifs_off >> end;
        _offsetLib[id] = std::make_pair(begin, end);
    }
    ifstream ifs_page;
    ifs_page.open("../offline/page/data/ripepage.dat");
    cout << "loading ripepage.dat ..." << endl;
    for(auto it = _offsetLib.begin(); it != _offsetLib.end(); ++it) {
        int begin = it->second.first;
        const size_t size = it->second.second;
        char buf[65536] = {0};
        string line;
        ifs_page.seekg(begin);
        ifs_page.read(buf, size);
        string doc = buf;
        _pageLib.insert(std::make_pair(it->first, webPage(doc, &_jieba, &_sim)));
    }

    ifstream ifs_index;
    ifs_index.open("../offline/page/data/invertIndex.dat");
    cout << "loading invertIndex.dat ..." << endl;
    string line2;
    string word;
    while(getline(ifs_index, line2)) {
        istringstream iss(line2);
        iss >> word;
        int docid;
        while (iss >> docid)
        {
           double w;
           iss >> w;
           _inverIndexTable[word].insert(std::make_pair(docid, w));
        }
        
    }
   
    ifs_off.close();
    ifs_page.close();
    ifs_index.close();
}
};//end of namespace search_engine








