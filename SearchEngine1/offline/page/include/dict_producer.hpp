#ifndef __SE_DICTPRODUCER_H__
#define __SE_DICTPRODUCER_H__

#include "word_segmentation.h"

namespace search_engine
{

class DictProducer
{
public:
    DictProducer()
    : _jieba()
    {}
    ~DictProducer(){  }
    WordSegmentation & getJieba() {
        return _jieba;
    }
private:
    WordSegmentation                _jieba;
};

};//end of namespace set_exception_at_thread_exit
#endif  //__SE_DICTPRODUCER_H__
