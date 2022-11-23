/*  File Name      dir_scanner.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-29 22:45
 *  Last Modified  2022-07-30 14:14
 */
#ifndef __SE_DIRSCANNER_H__
#define __SE_DIRSCANNER_H__

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace search_engine {

class DirScanner{
public:
    DirScanner();
    ~DirScanner();
    void operator()(const string& dirname);
    vector<string>& files();
    void traverse(const string& dirname);
private:
    vector<string> _files;
};

} // namespace search_engine

#endif // __SE_DIRSCANNER_H__
