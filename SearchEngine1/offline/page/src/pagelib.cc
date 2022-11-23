/*  File Name      pagelib.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 22:59
 * Last Modified 2022-10-28 10:52
 */

#include "../include/pagelib.h"

#include <math.h>
#include <fstream>
#include <regex>
#include <iostream>
#include <fstream>
#include <map>

#include "../include/tinyxml2.h"
#include "../include/dir_scanner.h"
#include "../include/configuration.hpp"
#include "../include/simhash/Simhasher.hpp"
using std::cout;
using std::endl;
using std::ofstream;
using std::map;
using namespace tinyxml2;

namespace search_engine {


PageLib::PageLib() 
: _config(Configuration::getInstance("conf/page.conf"))
, _jieba(_dict.getJieba())
{
    create();
    cout << "网页库信息采集成功！服务器加油中！！！" << endl;
    Page_De_Duplication(); //网页去重
    cout << "网页去重已完成！" << endl;
    handle_Invert_Index();       //建立倒排索引
    cout << "倒排索引建立成功！距离胜利不远了！" << endl;
    store();  //持久化网页库、偏移库、倒排索引表 << endl;
    cout << "任务结束，此次创建网页信息完成！！！" << endl;
}

void PageLib::create() {
    DirScanner dir;
    dir(_config->get_configs().find("XMLDIR")->second);
    for (string & filename : dir.files()) {
        XMLDocument doc;
        doc.LoadFile(filename.c_str());
        if (doc.ErrorID()) {
            perror("loadFile fail");
            return;
        }
        XMLElement *itemNode = 
            doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
        while(itemNode) {
            RSSIteam rssItem;
            std::regex reg("<[^>]+>");//通用正则表达式

            if(itemNode->FirstChildElement("title")) {
                string title = itemNode->FirstChildElement("title")->GetText();
                title = regex_replace(title, reg, "");
                rssItem._title = title;
            }else {
                rssItem._title = "这篇文档没有标题Qaq";
            }
            
            string link =  itemNode->FirstChildElement("link")->GetText();
            rssItem._link = link;
            if(itemNode->FirstChildElement("description"))
            {
                string description = itemNode->FirstChildElement("description")->GetText();
                description = regex_replace(description, reg, "");
                rssItem._description = description;
            }else {
                rssItem._description = "这篇文档没有内容Qaq!";
            }
            _rss.push_back(rssItem);
            itemNode = itemNode->NextSiblingElement("item");
        }
    }
}

void PageLib::Page_De_Duplication() {
    size_t topN = 20;
    simhash::Simhasher simhasher(DICT_PATH, HMM_PATH, IDF_PATH, STOP_WORD_PATH);
    vector<pair<int, uint64_t>> Sim;
    for(size_t i = 0; i < _rss.size(); ++i) {
        uint64_t u = 0;
        simhasher.make(_rss[i]._description, topN, u);
        Sim.push_back(std::make_pair(i, u));
    }
    set<int> tmp;
    for(size_t it = 0; it < Sim.size(); ++it) {
        int id1 = Sim[it].first;
        int sz1 = _rss[id1]._description.size();
        for(size_t it2 = (it + 1); it2 < Sim.size(); ++it2){
            if(simhash::Simhasher::isEqual(Sim[it].second, Sim[it2].second)){
                int sz2 = _rss[Sim[it2].first]._description.size();
                if(sz1 >= sz2 ) {
                    tmp.insert(Sim[it2].first);
                }else{
                    tmp.insert(id1);
                    break;
                }
            }
        }
    }

    int s = 0;
    for(auto num : tmp) {
        num -= 0;
        int i = 0;
        for(auto it = _rss.begin(); it != _rss.end(); ++it, ++i) {
            if(num == i) {
                _rss.erase(it);
                ++s;
                break;
            }
        }
    } 
    _DOCICNUM = _rss.size();
}
void PageLib::handle_Invert_Index()
{
    unordered_map<string, unordered_map<int, double>> tf; //每篇文档中的词频统计
    int i = 0;
    for(auto it = _rss.begin(); it != _rss.end(); ++it){
        int docid = ++i;
        string doc = it->_description;
        vector<string> results = _jieba(doc);
        for(string & word : results) {
            auto & isExit = tf[word];
            if(isExit[docid]) {
                ++isExit[docid];
            }else {
                isExit[docid] = 1;
            }
        }
    }
    // hello (1, 3)  (2, 10) (3, 7)
    unordered_map<int, double> docW;  // 存放1  w1^2 + w2^2...;  
    for(auto it = tf.begin(); it != tf.end(); ++it) {
        string word = it->first;       //hello
        size_t DF = it->second.size(); //3
        for(auto num = it->second.begin(); num != it->second.end(); ++num){
            int docid = num->first;
            int TF = num->second;
            double IDF = log(1.0 * _DOCICNUM /(DF + 1))/log(2);
            double w = TF * IDF;
            num->second = w;
            docW[docid] += w * w;  
        }
    }
    //已经得到 hello (1, w)  (2, w) (3, w)
    //  1  ALLW
    for(auto it = tf.begin(); it != tf.end(); ++ it) {
        string word = it->first;
        for(auto elem = it->second.begin(); elem != it->second.end(); ++elem) {
            int docid = elem->first;
            double sumW2 = docW[docid];
            double w = elem->second;
            double FinW = w / sqrt(sumW2);
            _invertIndex[word].insert(std::make_pair(docid, FinW));
        }

    }
}
void PageLib::store() {
    /* _dict.store(); */
    ofstream ofs("data/ripepage.dat"); //网页库
    if(!ofs) { 
        perror("ofs fail");
        return;
    }
    ofstream ofs1("data/offset.dat"); //网页偏移库
    if(!ofs1) { 
        perror("ofs fail");
        return;
    }
    for(size_t i = 0; i != _rss.size(); ++i) {
        size_t idx = i + 1;
        ofs1 << idx << " ";
        size_t  beginpos = ofs.tellp();
        ofs1 << beginpos << " ";
        ofs << "<doc>"<<endl;
        ofs << "\t<docid>" << i + 1 << "</docid>" <<endl;
        ofs << "\t<title>" << _rss[i]._title << "</title>" <<endl;
        ofs << "\t<url>" << _rss[i]._link << "</url>" <<endl;
        ofs << "\t<description>" << _rss[i]._description<< "</description>" <<endl;
        ofs << "</doc>" <<endl;
        size_t endpos = ofs.tellp();
        size_t len = endpos - beginpos;
        ofs1 << len << "\n";
    }
    cout << "网页库和网页偏移库持久化完成" << endl;
    ofstream ofs3;
    ofs3.open("data/invertIndex.dat");
    for(auto & it : _invertIndex) {
        ofs3 << it.first << " ";
        for(auto it2 = it.second.begin(); it2 != it.second.end(); ++it2) {
            ofs3 << it2->first << " " << it2->second << " " ;
        }
        ofs3 << endl;
    }
    cout << "倒排索引库持久化完成" << endl;
    ofs1.close();
    ofs.close();
    ofs3.close();

}
#if 0
void PageLib::handle_wordMap() {
    for(auto it = _docid.begin(); it != _docid.end(); ++it) {
        vector<string> results = _jieba(it->second);
        for(string & word : results) {
            auto & it2 = _wordMap[it->first];
            int & isExit = it2[word];
            if(isExit) {
                ++isExit;
            }else {
                isExit = 1;
            }
        }
    }
}

void PageLib::handle_topK() {
    size_t topN = 20;
    simhash::Simhasher simhasher(DICT_PATH, HMM_PATH, IDF_PATH, STOP_WORD_PATH);
    for(auto it = _docid.begin(); it != _docid.end(); ++it) {
        vector<pair<string, double>> results;
        simhasher.extract(it->second, results, topN);
        for(auto & it2 : results) {
            _topK[it->first].push_back(it2.first);
        }
    }
}
#endif

} // namespace search_engine



