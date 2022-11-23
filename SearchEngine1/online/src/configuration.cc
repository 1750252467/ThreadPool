/*  File Name      configuration.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-02 11:55
 * Last Modified 2022-09-16 15:28
 */
#include "../include/configuration.h"


namespace search_engine {

Configuration* Configuration::pInstance = nullptr; // 饱汉模式，因为是离线部分不用考虑多线程安全

/* string Configuration::_config_file_path = "conf/server.conf";       // 配置文件路径 */

Configuration* Configuration::getInstance() { // 静态成员函数
    if (!pInstance) {
        pInstance = new Configuration("../conf/server.conf");          
    }
    return pInstance;
}


Configuration::Configuration(const string& file_path) {
    // 初始化配置文件部分
    ifstream ifs;
    ifs.open(file_path);
    if (!ifs.good()) { std::cerr << "open server.conf fail" << endl; }

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
