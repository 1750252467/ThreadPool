/*  File Name      key_recommander.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-01 11:35
 *  Last Modified  2022-08-04 02:05
 */
#include "../../include/KeyRecommander/key_recommander.h"

#include <string.h>

#include <iostream>

namespace search_engine {


vector<string> KeyRecommander::doQuery() {
    vector<std::pair<string, int>> candit = _pdict->doQuery(_sought);
    vector<string> finalResult;

    if (!candit.size()) return vector<string>();
    Res myResult;
    for (auto& elem : candit) {
        if(3 == _pdict->nBytesCode(elem.first[0])) {//是中文词组 
            insertPrique(elem,myResult);
        }
    }
    initFinalResult(finalResult);

    for (auto& elem : candit) {
        if(1 == _pdict->nBytesCode(elem.first[0])) { //是英文词组
            insertPrique(elem,myResult);
        }
    }
    initFinalResult(finalResult);
    eraseFinalResult(finalResult);

    return finalResult;
}

void KeyRecommander::insertPrique(std::pair<string,int> & elem ,Res & myResult) {
    bzero(&myResult, sizeof(myResult));
    myResult._dist = _pdict->editDistance(_sought, elem.first);
    myResult._freq = elem.second;
    myResult._word = elem.first;
    _prique.push(myResult);
}

void KeyRecommander::initFinalResult(vector<string> & finalResult) {
    int i= 0;
    while (!_prique.empty() && i++ < 5) {
        finalResult.emplace_back(_prique.top()._word);
        _prique.pop();
    }
    while (_prique.size()) {
        _prique.pop();
    }
}

void KeyRecommander::eraseFinalResult(vector<string> & finalResult) {
    size_t resultSize= finalResult.size();
    if(resultSize > 5) {
        auto it = finalResult.begin();
        finalResult.erase(it+3,it+5);
        finalResult.erase(it+6,it+8);
    }
}



bool KeyRecommander::myCompare::operator()(const CandidateResult & lhs, 
                           const CandidateResult & rhs) const {
    if(lhs._dist > rhs._dist) {//距离更小的优先级高
        return true;
    }
    else if (lhs._dist == rhs._dist && lhs._freq < rhs._freq) {//频率高的优先级高
        return true;
    }
    else if (lhs._dist == rhs._dist && lhs._freq == rhs._freq) {

        if (strcmp(lhs._word.c_str(),rhs._word.c_str()) > 0) { //英文字母顺序更小的优先级高
            return true;
        }
        else { return false; }
    }
    else { return false; }
}


} // namespace search_engine
