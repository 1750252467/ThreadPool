#ifndef _NONCOPYABLE_H__
#define _NONCOPYABLE_H__

class NonCopyable
{
protected:
    NonCopyable() {}
    ~NonCopyable() {}

    NonCopyable(const NonCopyable &rhs) = delete;
    NonCopyable &operator=(const NonCopyable &rhs) = delete;

};  



#endif