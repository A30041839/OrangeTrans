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

#pragma once
#include "GlobalFun.h"
#define  PROB_FILENAME "trigram_pro1.txt"
#define RES_PROB_FILENAME "rest_pro.txt"

namespace OrangeLM
{
  class AbsoluteDiscounting
  {
  private:
    GramProb m_GramPro; //!����n_gram�ĸ���
    double m_restPro; //!ʣ��ĸ���

  public:
    /*
    *���þ��Լ�ֵ������gram�ĸ���
    *gramMapListΪ�����n-gram��ͳ�ƴ�Ƶ
    *gramProbListΪ�����n-gramͳ�Ƹ���
    *allocatProΪ�������ǰgram���ܸ���
    *restProΪ�������ǰgram��ʣ��ĸ���
    */
    void UseAbsoluteDiscounting(GramListMap &gramMapList, int vocabularySize, string savePath);

    //��ȡn-gram���ʵ��б�

    vector<GramProb> GetGramProVec();

    //���������sentence���д��
    double ScoreSentence(string sentence);

    //�������gram���д��
    double ScoreGram(vector<string> gramVec);

    /*
    *����ָ��·��gramProSavePath�µĸ���map ��gramList��
    */
    void LoadGramPro(const string gramProSavePath);

    AbsoluteDiscounting(void);
    ~AbsoluteDiscounting(void);
  };
}


