/*  File Name      disallow_copy_assign.h
 *  Author         wd_cpp44th_group7th
 *  Gitee          https://gitee.com/magician-007
 *  Created Time   2022-07-28 20:32
 *  Last Modified  2022-07-30 12:24
 */
#ifndef __SE_DISALLOW_COPY_ASSIGN_H__
#define __SE_DISALLOW_COPY_ASSIGN_H__

namespace search_engine{

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&);         \
        TypeName& operator=(const TypeName&)

} // namespace search_engine

#endif // __SE_DISALLOW_COPY_ASSIGN_H__
