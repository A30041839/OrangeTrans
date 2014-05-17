/**
*Orange Trans: A statistical machine translation decoder: Phrase-based translation
*and Hiero Phrase-based translation.
*
*$Id:
*$Version:
*0.0.1
*$Created by:
*Xiong Lv
*$Last Modified:
**/

/*
*WordMap����Ҫ��������n-gram�Ĵ�������Ƶ
*
*/

#pragma once
#include "GlobalFun.h"
#include <fstream>

namespace OrangeLM
{
  class WordMap
  {
  private:
    GramListMap m_uniGramList; //!uniGram ͳ�ƴ�Ƶ
    GramListMap m_biGramList;  //!biGram ͳ�ƴ�Ƶ
    GramListMap m_triGramList; //!triGram ͳ�ƴ�Ƶ
    vector<GramListMap> m_GramVec;//!����gram��ͳ�ƴ�Ƶ


    void WordMap::GenerateNGramMap(string &text);
    void WordMap::ExtractNGram(
      vector<string> &wordsVec, //��ȡ�ĵ�������
      int n_gram, //ʹ��nԪͳ��
      GramListMap &gramList//nԪͳ�ƴ�Ƶmap
      );
    bool  WriteGramFreqToFile(const string &gramFreqSavePath); //��ͳ�Ƶ�3��gram��Ƶд���ļ�

  public:
    //��������Ҳ���ǵ�gram ����
    void GetNNramMapIgnoreIcon(string &text);
    //��ָ���ĵ����г�ȡgram��Ƶ

    void DealCorpus(const string &corpusPath, const string &gramFreqSavePath);
    GramListMap GetNGramMap(int &n_gram); //����Ϊ���ص�n_gram��gramMap
    WordMap(void);
    ~WordMap(void);
  };
}



