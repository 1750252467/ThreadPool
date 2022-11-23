#include "../include/dict_producer.h"

#include <fstream>

using std::ifstream;
using std::ofstream;

namespace search_engine
{

DictProducer::DictProducer()
: _jieba()
, _dir()
{   
    _dir("data/CN/art");
    for(auto & fileName : _dir.files()){
        _file.push_back(fileName);
    }

    create_Cn_Dict();
    create_En_Dict();
    store();
}

DictProducer::~DictProducer()
{

}


void DictProducer::create_Cn_Dict()
{
    for(string & fileName : _file){
        cout << " aaa "<<  fileName << endl;
        ifstream ifs;
        open_File(ifs, fileName);
        string line;
        while(ifs >> line){
            vector<string> results = _jieba(line);
            for(string & elem : results){
                if(elem.size() % CHSIZE == 0){  //汉字组
                    int & isExit = _dict[elem];
                    if(isExit){
                        ++isExit;
                    }else{
                        isExit = 1;
                        size_t id = _dict.size();
                        _idMap[id] = elem;
                        insert_Index(elem, id);
                    }
                }
            }
        }
        ifs.close();
    }
}

void DictProducer::insert_Index(const string & elem, size_t id)
{
    vector<string> results;
    _jieba.CutSmall(elem, results, 1);
    for(string & it : results){
        _index[it].insert(id);
    }
}

void DictProducer::store()
{
    ofstream ofs_dict;
    input_File(ofs_dict, "data/dict.dat");
    for(auto it = _dict.begin(); it != _dict.end(); ++it ){
        ofs_dict <<  it->first << " " << it->second << endl;
    }
    
    ofstream ofs_index;
    input_File(ofs_index, "data/index.dat");
    for(auto it = _index.begin(); it != _index.end(); ++it ){
        ofs_index <<  it->first << " " ;
        for(auto & set : it->second ){
            ofs_index << set << " ";
        }
        ofs_index << endl;
    }
#if 1
    ofstream ofs_idMap;
    input_File(ofs_idMap, "data/idMap.dat");
    for(auto it = _idMap.begin(); it != _idMap.end(); ++it ){
        ofs_idMap <<  it->first << " " << it->second << endl;
    }
    ofs_idMap.close();
#endif
    ofs_dict.close();
    ofs_index.close();
}



void DictProducer::create_En_Dict()
{
    ifstream ifs;  
    open_File(ifs,"data/EN/english.txt");
    string line;   
    while(getline(ifs,line)){                  
        processLine(line);
        istringstream iss(line);
        string word;                                         
        while(iss >> word){   
            int & isExit = _dict[word];
            if(isExit){
                ++isExit;
            }else{
                isExit = 1;
                size_t id = _dict.size();
                _idMap[id] = word;
                insert_En_index(word, id);
            }
        }    
    }    
    ifs.close();
}

void DictProducer::insert_En_index(const string & word, size_t id)
{
    for(size_t  i = 0; i < word.size(); ++i){              
        string ch(1,word[i]);
        _index[ch].insert(id);
    }           
}

void DictProducer::processLine(string & line)
{
    for(auto & elem : line){
        if(!isalpha(elem)){
            elem = ' ';
        }
        else if(isupper(elem)){
            elem = tolower(elem);
        }
    }
}

void DictProducer::open_File(ifstream &ifs, const string & fileName)
{
    ifs.open(fileName);
    cout << "open >>>" << fileName << endl;
    if(!ifs){
        perror("open file failed in Dict ifs");
    }
}

void DictProducer::input_File(ofstream & ofs, const string & fileName)
{
    ofs.open(fileName);
    if(!ofs){
        perror("open file failed in Dict ifs");
    }
}

}; // end of namespace search_engine
