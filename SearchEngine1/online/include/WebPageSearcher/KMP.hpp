#ifndef __SE_KMP_H__
#define __SE_KMP_H__

#include "string.h"
#include <string>
#include <set>
#include <iostream>

using std::string;
using std::set;

namespace search_engine
{

class KMP
{
public:
    int operator()(const string &s, const string ss)
    {
        int pos = kmp(s.c_str(), ss.c_str());
        return pos;
    }

    int* getNext(string p)
    {
        int* next = new int[p.length()];
        next[0] = -1;
        int j = 0;
        int k = -1;
        while (j < (int)p.length() - 1)
        {
            if (k == -1 || p[j] == p[k])
            {
                j++;
                k++;
                next[j] = k;
            }
            else
            {
                k = next[k];
            }
        }
        return next;
    }

    int kmp(string T,string p)
    {
        int i=0;
        int j=0;
        int* next=getNext(T);
        while (i < (int)T.length() && j < (int)p.length())
        {
            if (j == -1 || T[i] == p[j])
            {
                i++;
                j++;
            }
            else
            {
                j=next[j];
            }
        }
        if (j == (int)p.length())
        {

            delete []next;
            return i-j;
        }
        delete []next;
        return -1;
    }
};



};// end of namespace search_engine


#endif // end of __SE_KMP_H__
