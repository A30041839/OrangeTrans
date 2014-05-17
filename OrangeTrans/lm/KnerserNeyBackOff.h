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
/*
*Knerser-Ney����ƽ�����������õ������ƽ����ʽ
*
*/
//����maptree ���ڸ�Ч����n-1-gram_* ������
//ͳ��map ��ֵstring��ʼ����Ӧλ�õĴ�Ƶͳ����Ϣ

namespace OrangeLM
{
  typedef struct{
    int count;
    int freq_N[3];
  }Stat;
  typedef struct{
    map<string, Stat> bigram;
    Stat stat;
  }UnigramMap;

  typedef map<string, UnigramMap> UnigramMapTree;


  class KnerserNeyBackOff
  {
  private:
    vector<GramListMap> m_GramMapVec; //! the probability of n-gram

    vector<GramProbBow> m_GramProBack;//! the probability of n-gram with backoff weight;

  public:

    //���������˸��ʵ�n_gram����
    //caculate the n_gram with backoff-weight probability
    void CaculateNGramProBow(vector<GramListMap> &gramMapVec, string &proSavePath);
    //����n_gram����
    //caculate the probability of n_gram
    void CaculateGramProVec(vector<GramListMap> &gramMapVec, string & proSavePath);

    //���þ��Լ�ֵ�� ���� ����ͽ�gram�ĸ���
    //
    void AbsoluteDiscounting(GramProb &gramProbList, GramListMap &gramMapList, double allocatePro, double &restPro);

    //����gram���ʱ�
    //load the n_gram probability Map
    void LoadGramPro(const string gramProSavePath);

    //�ͷ�������Դ
    //release all source
    void ReleaseSource();

    //��ȡn-gram���ʵ��б�
    //get the n-gram probability
    vector<GramProbBow> GetGramProVec();

    //�����������������Ϣ�ľ���sentence���д��
    //score the sentence with context
    double ScoreSentenceWithContext(vector<string> sentence);


    //��������������Ϣ��sentence���
    //score the sentence without context
    double ScoreSentenceWithoutContext(vector<string> sentence);

    //�������gram���д��
    //socre the n-gram
    double ScoreGram(vector<string> gramVec);

    //��src�е�pos���ո����ַ��滻Ϊiterm
    void  ReplaceMapString(string src, string &des, int pos);

    //����ָ��gram��εĸ��ʺ���ͽ׵�bow
    //caculate the probability of n_gram  and the backoff weights of n-1 gram
    void CaculateBowAndPkn(GramListMap &gramList,
      GramListMap &vocabulary,
      GramProbBow &gramProb,
      GramProbBow &lowGramBow,
      UnigramMapTree &uniGramMapTree,
      int order);



    //���������gramƵ�� ������discount
    //caculate the discount of the input gram_frequency

    void  CaculateDiscount(GramListMap &gramList, double *discount);

    KnerserNeyBackOff(void);
    ~KnerserNeyBackOff(void);
  };


}