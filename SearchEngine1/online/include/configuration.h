/*  File Name      configuration.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 23:00
 *  Last Modified  2022-08-03 10:41
 */
#ifndef __CONFIGURATION__
#define __CONFIGURATION__

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
    static Configuration* getInstance(); 
    map<string, string>& get_configs();     // 获取存放配置文件内的map
    /* set<string> get_stop_word_list();       // 获取停用词集 */
    ~Configuration() {
        if (pInstance) {
            delete pInstance;
            pInstance = nullptr;
        }
    }

private:
    explicit Configuration(const string& filePath); // 构造函数私有化

private:
    DISALLOW_COPY_AND_ASSIGN(Configuration);
    
    static Configuration* pInstance;
    static string _config_file_path;       // 配置文件路径
    map<string, string> _configs;   // 配置文件内容
    /* set<string> _stop_word_list;    // 停用词词集 */ 
};



} // namespace search_engine


#endif // __CONFIGURATION__

