/*  File Name      dictionary.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-01 11:53
 * Last Modified 2022-09-16 16:01
 */
#include "../../include/KeyRecommander/dictionary.h"

#include <stdio.h>
#include <assert.h>

#include "../../include/configuration.h"

namespace search_engine {
    Dictionary* Dictionary::_pdict = nullptr;

    map<string, string> configs = 
    Configuration::getInstance()->get_configs();

Dictionary* Dictionary::getInstance() {
    if (nullptr == _pdict) {
            _pdict = new Dictionary();
            _pdict->loadDict();
            _pdict->loadIndex();
            _pdict->loadIdMap();
        }
        return _pdict;
}

void Dictionary::destroy() {
    if (_pdict) {
        delete _pdict;
        _pdict = nullptr;
    } 
}

vector<std::pair<string, int>> Dictionary::doQuery(const string& sought) {
    vector<string> words; 
    handleWord(sought, words); //words中存储的是一个个的汉字或者字母
    
    unordered_map<string,set<int>> word_idx;
    for (auto & elem : words) {
        if (_index.find(elem) != _index.end()) {//索引表中存在该elem
            word_idx.insert(make_pair(elem, _index[elem]));
        } 
    }

    set<int> ids;
    for (auto& it : word_idx) {
        for (auto& it2 :it.second) {
            ids.insert(it2);
        }   
    }

    string word;
    int freq;
    vector<pair<string, int>> similar_word;
    /* similar_word.clear(); */
    for (auto& it : ids) {
        word = _idMap[it];
        freq = _dict[word];
        similar_word.push_back(make_pair(word, freq));
    }    //根据用户给到的单词查询
    return similar_word;
}

void Dictionary::handleWord(const string& input, vector<string>& words) {
    int len = input.length(),i = 0;
    while (i < len) {
        assert((input[i] & 0xF8) <= 0xF0);
        int next = 1;
        if((input[i] & 0x80) == 0x00) {
        }  else if ((input[i] & 0xE0) == 0xC0) {
            next = 2;
        } else if ((input[i] & 0xF0) == 0xE0) {
            next = 3;
        } else if ((input[i] & 0xF8) == 0xF0) {
            next = 4;
        }
        words.push_back(input.substr(i, next));
        i += next;
    }
}


size_t Dictionary::nBytesCode(const char ch) {
	if(ch & (1 << 7))
	{
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;	
			}
			else
				break;
		}
		return nBytes;
	}
	return 1;
}

size_t Dictionary::length(const std::string &str) {
    size_t ilen = 0;
	for(size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += (nBytes - 1);
		++ilen;
	}
	return ilen;
}

int Dictionary::editDistance(const string& lhs, const string &rhs) {
	size_t lhs_len = length(lhs);
	size_t rhs_len = length(rhs);
	int editDist[lhs_len + 1][rhs_len + 1];
	for(size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}

	for(size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	
	std::string sublhs, subrhs;
	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		size_t nBytes = nBytesCode(lhs[lhs_idx]);
		sublhs = lhs.substr(lhs_idx, nBytes);
		lhs_idx += (nBytes - 1);

		for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			nBytes = nBytesCode(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += (nBytes - 1);
			if(sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
			else
			{
				editDist[dist_i][dist_j] = triple_min(
					editDist[dist_i][dist_j - 1] + 1,
					editDist[dist_i - 1][dist_j] + 1,
					editDist[dist_i - 1][dist_j - 1] + 1);
			}
		}
	}
	return editDist[lhs_len][rhs_len];
}

int Dictionary::triple_min(const int& a, const int& b, const int& c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}



void Dictionary::loadDict() {
    string dict = configs[string("dict")];
   
    ifstream ifs(dict);
    // string s = "../../../offline/word/data/dict.dat";
    if (!ifs) {
        cerr << "open dict.dat failed" << endl;
        return;
    }
    cout << "loading dict.dat ..." << endl;
    string line, word, freq;
    while (getline(ifs, line)) {
        istringstream is(line);
        is >> word >> freq;
        _dict.insert(std::pair<string, int>(word, stoi(freq)));
    }
    ifs.close();
}

void Dictionary::loadIndex() {
    string index = configs["index"];
    ifstream ifs(index);
    if (!ifs) {
        cerr << "open index.dat failed" << endl;
        return;
    }
    cout << "loading index.dat ..." << endl;
    string line, one,  wordId;
    while (getline(ifs, line)) {
        istringstream is(line);
        is >> one;
        while (is >> wordId) {
            _index[one].insert(stoi(wordId));      
        }
    }
    /* for(auto& it : _index) { */
    /*       cout << it.first << " " ; */
    /*       for (auto& it2 : it.second) { */
    /*           cout << it2 << " "; */
    /*       } */
    /*       cout << "\n"; */
    /* } */
    /* ifs.close(); */
}

void Dictionary::loadIdMap() {
    string idMap = configs["idMap"];
    ifstream ifs(idMap);
    if (!ifs) {
        cerr << "open idMap.dat failed" << endl;
        return;
    }
    cout << "loading idMap.dat ..." << endl;
    string line, wordId, word;
    while (getline(ifs, line)) {
        istringstream is(line);
        is >> wordId >> word;
        _idMap.insert(std::pair<int, string>(stoi(wordId), word));
    }
    /* for(auto& it : _idMap) { */
    /*       cout << it.first << " " << it.second << endl; */
    /* } */
    /* ifs.close(); */
}



}     // namespace search_engine




