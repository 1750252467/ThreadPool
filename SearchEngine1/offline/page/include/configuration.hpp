/*  File Name      configuration.hpp
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 23:00
 *  Last Modified  2022-07-31 23:45
 */
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

#include "disallow_copy_assign.h"

using std::string;
using std::map;
using std::set;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::pair;
using std::cerr;

namespace search_engine {

// 单例模式
class Configuration {
public:
    static Configuration* getInstance(const char* file_path); 
    map<string, string>& get_configs();     // 获取存放配置文件内å[MBY7¹的map 
    /* set<string> get_stop_word_list();       // 获取停用词集 */

private:
    explicit Configuration(const string& file_path); // 构造函数私有化
    ~Configuration();

private:
    DISALLOW_COPY_AND_ASSIGN(Configuration);
    
    static Configuration* pInstance;
    string _config_file_path;       // 配置文件路径
    map<string, string> _configs;   // 配置文件内容
    /* set<string> _stop_word_list;    // 停用词词集 */ 
};

Configuration* Configuration::pInstance = nullptr; // 饱汉模式，因为是离线部分不用考虑多线程安全

Configuration*  Configuration::getInstance(const char* file_path) { // 静态成员函数
    if (!pInstance) {
        pInstance = new Configuration(string(file_path));          
    }
    return pInstance;
}

Configuration::Configuration(const string& file_path) 
    : _config_file_path(file_path) {
    // 初始化配置文件部分
    ifstream ifs;
    ifs.open(_config_file_path);
    if (!ifs.good()) { std::cerr << "open en_file_dir faile" << endl; }

    string line, key, val;
    while (getline(ifs, line), !ifs.eof()) {
        istringstream iss(line); 
        iss >> key;
        iss >> val;
        pair<string, string> record (key, val);
        _configs.insert(record);
    }
    ifs.close();
}

map<string, string>& Configuration::get_configs() {
    return _configs;
}

} // namespace search_engine

#endif // __CONFIGURATION_H__


