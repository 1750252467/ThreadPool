#include "../include/word_segmentation.h"

vector<string> search_engine::WordSegmentation::operator()(const string str)
{
    vector<string> words;
    _jieba.CutAll(str, words); //FullSegment
    return words;
}

void search_engine::WordSegmentation::CutSmall(const string & str, vector<string> & vec, size_t sz)
{
    _jieba.CutSmall(str, vec, sz);
}
