/*  File Name      web_page_searcher.cc
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-08-01 20:01
 *  Last Modified  2022-08-01 20:01
 */
#include "../../include/WebPageSearcher/web_page_searcher.h"


/* #include "../../include/WebPageSearcher/web_page_query.h" */
namespace search_engine {


/* WebPageSearcher::WebPageSearcher(string key, const TcpConnection & conn) */
/*     : _sought(key) */
/* { */
/*     /1* _conn = conn; *1/ */
/* } */
void WebPageSearcher::set_m_val(string sought, TcpConnectionPtr con) {
    _sought = sought;
    _conn = con;
}

WebPageSearcher::WebPageSearcher()
    : _pQuery(web_page_query::get_Web_Query_ptr()) {}

WebPageSearcher::WebPageSearcher(string key)
   : _sought(key)
{
    _pQuery = web_page_query::get_Web_Query_ptr();
    /* _conn = conn; */
}
vector<string> WebPageSearcher::doQuery() {
    vector<string> result = _pQuery->doQuery(_sought);
    return result;
}



} // namespace search_engine
