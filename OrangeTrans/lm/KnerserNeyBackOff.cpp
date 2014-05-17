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
#include "KnerserNeyBackOff.h"

namespace OrangeLM
{
  KnerserNeyBackOff::KnerserNeyBackOff(void)
  {

  }


  KnerserNeyBackOff::~KnerserNeyBackOff(void)
  {
    m_GramMapVec.clear();
    m_GramProBack.clear();
  }
  /*
  *���þ��Լ�ֵ������ͽ�gram�ĸ���
  *gramMapListΪ�����n-gram��ͳ�ƴ�Ƶ
  *gramProbListΪ�����n-gramͳ�Ƹ���
  *allocatProΪ�������ǰgram���ܸ���
  *restProΪ�������ǰgram��ʣ��ĸ���
  */
  void KnerserNeyBackOff::AbsoluteDiscounting(GramProb &gramProbList,
    GramListMap &gramMapList,
    double allocatePro,
    double &restPro
    )
  {
    //GramProb gramProbList;
    int totalGramNum = 0; //��������Gram����
    double discountNum; //�ۿ���
    double restProTri = 0.0; //Gram���¼�����ʺ��ʣ�����
    int n1 = 0, n2 = 0;  //ͳ����Gram�г��ִ���Ϊ1�κ����ε��¼�����
    int gramListNum = gramMapList.size();
    GramListMap::iterator itr_gram = gramMapList.begin();
    GramListMap::iterator itr_gram_end = gramMapList.end();
    for (; itr_gram != itr_gram_end; itr_gram++)
    {
      totalGramNum += itr_gram->second;
      if (itr_gram->second == 1)
      {
        n1++;
      }
      else if (itr_gram->second == 2)
      {
        n2++;
      }
      else
      {
        continue;
      }

    }
    discountNum = (double)n1 / (n1 + 2 * n2);  //ʹ�����޴����Ż���dicountNum

    for (itr_gram = gramMapList.begin(); itr_gram != itr_gram_end; itr_gram++) //���¼����ֵ��Gram��ÿ���¼��ĸ���
    {

      gramProbList[itr_gram->first] = log10f((double)allocatePro*(((double)itr_gram->second - discountNum) / totalGramNum));
    }
    restPro = allocatePro*(1.0 - (double)(discountNum*gramListNum) / totalGramNum); //����Gramʣ�����
    return;

  }
  //���������˸��ʵ�n_gram����
  //caculate the n-gram with back-off weight
  void KnerserNeyBackOff::CaculateNGramProBow(vector<GramListMap> &gramMapVec, string &proSavePath)
  {
    int n1, n2, n3, n4;
    vector<GramListMap> ::iterator itr_gramVec = gramMapVec.begin();
    vector<GramListMap> ::iterator itr_gramVec_end = gramMapVec.end();
    GramListMap::iterator itr_gram;
    GramListMap::iterator itr_gram_end;
    UnigramMapTree unigramMapTree;
    Stat stat_info;
    stat_info.count = 0;
    stat_info.freq_N[0] = 0;
    stat_info.freq_N[1] = 0;
    stat_info.freq_N[2] = 0;
    itr_gram_end = gramMapVec[0].end();
    int voca_total_count = 0;
    for (itr_gram = gramMapVec[0].begin(); itr_gram != itr_gram_end; itr_gram++)
    {
      unigramMapTree[itr_gram->first].stat = stat_info;
      voca_total_count += itr_gram->second;
    }
    itr_gram_end = gramMapVec[1].end();
    string temp_bi;
    string temp_uni;

    for (itr_gram = gramMapVec[1].begin(); itr_gram != itr_gram_end; itr_gram++)
    {
      temp_bi.clear();
      ReplaceMapString(itr_gram->first, temp_bi, 1);

      if (unigramMapTree.count(temp_bi) > 0)
      {
        int count = itr_gram->second;
        unigramMapTree[temp_bi].stat.count += count;

        if (count >= 3)
          unigramMapTree[temp_bi].stat.freq_N[2]++;
        else if (count == 2)
          unigramMapTree[temp_bi].stat.freq_N[1]++;
        else if (count == 1)
          unigramMapTree[temp_bi].stat.freq_N[0]++;
        else
          continue;
      }
      temp_uni.clear();
      int pos = itr_gram->first.find_first_of(" ", 0);
      temp_uni = itr_gram->first.substr(pos + 1, itr_gram->first.size() - pos - 1);
      unigramMapTree[temp_bi].bigram[temp_uni] = stat_info;
    }
    itr_gram_end = gramMapVec[2].end();
    vector<string> wordVector;
    for (itr_gram = gramMapVec[2].begin(); itr_gram != itr_gram_end; itr_gram++)
    {
      wordVector.clear();
      StringToVector(itr_gram->first, wordVector);
      if (unigramMapTree[wordVector[0]].bigram.count(wordVector[1]) > 0)
      {
        int count = itr_gram->second;
        unigramMapTree[wordVector[0]].bigram[wordVector[1]].count += count;
        if (count >= 3)
          unigramMapTree[wordVector[0]].bigram[wordVector[1]].freq_N[2]++;
        else if (count == 2)
          unigramMapTree[wordVector[0]].bigram[wordVector[1]].freq_N[1]++;
        else if (count == 1)
          unigramMapTree[wordVector[0]].bigram[wordVector[1]].freq_N[0]++;
        else
          continue;

      }
    }
    double *discount = new double[N_GRAM];
    CaculateDiscount(gramMapVec[0], discount);
    GramProbBow gramProbBow;
    m_GramProBack.push_back(gramProbBow);
    itr_gram_end = gramMapVec[0].end();
    double temp = 0.0;
    double disount_C = 0.0;
    double voca_size = gramMapVec[0].size();
    for (itr_gram = gramMapVec[0].begin(); itr_gram != itr_gram_end; itr_gram++)  //����unigram�ļ�ֵ����
    {
      if (itr_gram->second >= 3)
        disount_C = discount[N_GRAM - 1];
      else
        disount_C = discount[itr_gram->second - 1];
      temp = double(itr_gram->second - disount_C) / voca_total_count;
      m_GramProBack[0][itr_gram->first].pKn = log10f(temp > 0 ? temp : 1);
    }

    for (int i = 1; i<N_GRAM; i++)  //��ʼ��bigram��trigram�ĸ���Map
    {
      GramProbBow gramProbBow;
      m_GramProBack.push_back(gramProbBow);
      itr_gram_end = gramMapVec[i].end();
      for (itr_gram = gramMapVec[i].begin(); itr_gram != itr_gram_end; itr_gram++)
      {
        m_GramProBack[i][itr_gram->first].pKn = 0.0;
      }
    }
    for (int i = N_GRAM - 1; i>0; i--)//����trigram�ļ�ֵ���� ����bigram�ļ�ֵ���ʺͺ���Ȩ��
    {
      CaculateBowAndPkn(gramMapVec[i], gramMapVec[0], m_GramProBack[i], m_GramProBack[i - 1], unigramMapTree, i);

    }

    WriteProbBowToFile(proSavePath, m_GramProBack);
    WriteProbSplitBowToFile(proSavePath, m_GramProBack);

  }
  /*
  *���������gramƵ�� ������discount
  *caculate the discount with specific gramList
  */
  void KnerserNeyBackOff::CaculateDiscount(GramListMap &gramList, double *discount)
  {
    int n[4] = { 0 };
    int total_count = 0;

    GramListMap::iterator itr_feq = gramList.begin();
    GramListMap::iterator itr_feq_end = gramList.end();

    //double discount[N_GRAM];
    for (; itr_feq != itr_feq_end; itr_feq++)//ͳ��n-gramÿ���¼����ֵĴ���
    {
      total_count++;
      if (itr_feq->second == 1)
        n[0]++;
      else if (itr_feq->second == 2)
        n[1]++;
      else if (itr_feq->second == 3)
        n[2]++;
      else if (itr_feq->second == 4)
        n[3]++;
    }
    double Y = ((double)n[0]) / (n[0] + 2 * n[1]);
    for (int i = 0; i < N_GRAM; i++)  //����discount
    {
      discount[i] = i + 1 - (i + 2)*Y*n[i + 1] / n[i];
    }
    cout << " finished to caculate the current discount!" << endl;
  }
  /*
  *����ָ��order�׵�״̬��
  *���ý���������map��
  *��ȡ��Ӧn-gram�Ĵ�Ƶͳ��Map�� ��Ӧ�׵�gramProbBow������Ӧ�׵ĸ���mapͬʱ������ͽ׺���Ȩ�أ�����lowGramBow��
  *vocabulary��ӦΪ�����ĵ����Ĵʻ㼯
  *int sepecific order use th map tree to caculate the gramProbBow and its lower-level gram back-off weights
  *store it to lowGramBow
  */
  void KnerserNeyBackOff::CaculateBowAndPkn(GramListMap &gramList,
    GramListMap &vocabulary,
    GramProbBow &gramPro,
    GramProbBow &lowGramBow,
    UnigramMapTree &uniGramMapTree,
    int order)
  {
    int n[4] = { 0 };

    int total_count = 0;

    double *discount = new double[N_GRAM];

    GramListMap::iterator itr_feq = gramList.begin();
    GramListMap::iterator itr_feq_end = gramList.end();

    int bigN[N_GRAM] = { 0 }; //store the gram counts

    cout << "begin to caculate the " << order << "-gram's prob and the " << order - 1 << "-gram 's bow ..." << endl;

    CaculateDiscount(gramList, discount);  //caculate the gramList ' discount in current order
    vector<string> wordVec;
    int count = 0;
    Stat stat_info;
    string temp_str;
    for (itr_feq == gramList.begin(); itr_feq != itr_feq_end; itr_feq++)
    {
      int count = 0;
      int total_WCount = 0;
      double discount_c = 0.0;
      if (itr_feq->second >= 3)
        discount_c = discount[N_GRAM - 1];
      else
        discount_c = discount[itr_feq->second > 0 ? itr_feq->second - 1 : 0];
      wordVec.clear();

      StringToVector(itr_feq->first, wordVec);
      temp_str.clear();
      if (order == 2) //is trigram get the statistic information of n-1 gram 
      {
        stat_info = uniGramMapTree[wordVec[0]].bigram[wordVec[1]];
        temp_str += wordVec[0] + " " + wordVec[1];
      }
      else if (order == 1) //is bigram
      {
        stat_info = uniGramMapTree[wordVec[0]].stat;
        temp_str += wordVec[0];
      }
      total_WCount = stat_info.count;
      double pKn = (double(itr_feq->second) - discount_c) / total_WCount;  //caculate pKn
      gramPro[itr_feq->first].pKn = log10f(pKn > 0 ? pKn : 1);
      double bow = 0.0;
      for (int i = 0; i<N_GRAM; i++) //caculate the back-off weights
      {
        bow += discount[i] * stat_info.freq_N[i];
      }
      bow /= total_WCount;
      if (lowGramBow.count(temp_str)>0) //assign the back-off weight to lower-level gram
        lowGramBow[temp_str].bow = log10f(bow > 0 ? bow : 1);

    }
    cout << "finished to caculate the " << order << "-gram's prob and the " << order - 1 << "-gram 's bow" << endl;

  }

  /*
  *����srcΪ������ַ���
  *desΪ�滻����ַ���
  *itermΪ�滻��Ԫ��
  *posΪ�滻����ʼλ��
  */
  void KnerserNeyBackOff::ReplaceMapString(string src, string &des, int pos) //��src�е�pos���ո����ַ��滻Ϊiterm
  {
    int i = pos;
    int start_pos = 0;
    des.clear();
    while (i > 0) //ѡ��ضϵ�λ�� ���нض�
    {
      start_pos = src.find(" ", start_pos);
      start_pos++;
      i--;
    }
    des = src.substr(0, start_pos - 1);
  }

  /*
  *��ȡn-gram���ʵ��б�
  *get the n-gram probability list
  */
  vector<GramProbBow> KnerserNeyBackOff::GetGramProVec()
  {
    return m_GramProBack;
  }

  /*
  *
  */
  void KnerserNeyBackOff::ReleaseSource()
  {
    m_GramProBack.clear();
    m_GramMapVec.clear();
  }
  /*
  *����ָ��·��gramProSavePath�µĸ���map ��gramList��
  *load the probability file in the specific path (gramproSavePath) to gramList
  */
  void KnerserNeyBackOff::LoadGramPro(const string gramProSavePath)
  {
    vector<string>gramNameText;
    gramNameText.push_back(UNIGRAM_FREQ_PATH);
    gramNameText.push_back(BIGRAM_FREQ_PATH);
    gramNameText.push_back(TRIGRAM_FREQ_PATH);

    GramProbBow gramList;
    string path;
    int nameLen = gramNameText.size();
    for (int i = 0; i < nameLen; i++)
    {
      m_GramProBack.push_back(gramList);
      cout << "����" << i + 1 << "-gramPro�ļ�..." << endl;
      path = gramProSavePath + gramNameText[i];
      GetGramPro(path, m_GramProBack[i]);
      //m_GramProVec.push_back(gramList);
      cout << "�ɹ�����" << i + 1 << "-gramPro�ļ�!" << endl;

    }
    return;
  }

  /*
  *��sentence��֣����ݹ���������ģ��
  *score the gram according to the lauguage model
  */
  double KnerserNeyBackOff::ScoreGram(vector<string> gramVec)
  {
    int gramLen = gramVec.size();
    string tempstr3, tempstr2, tempstr1;
    int i = 0;

    tempstr3 = gramVec[0] + " " + gramVec[1] + " " + gramVec[2];
    if (m_GramProBack[2].count(tempstr3) > 0)
    {
      return m_GramProBack[2][tempstr3].pKn; //find the 3-gram probability and renturn it's value

    }

    tempstr2 = gramVec[1] + " " + gramVec[2];
    tempstr1 = gramVec[0] + " " + gramVec[1];
    if (m_GramProBack[1].count(tempstr1) > 0) //the w1w2 2-gram exist
    {
      if (m_GramProBack[1].count(tempstr2) > 0)//w2w3 2-gram exist
        return m_GramProBack[1][tempstr1].bow + m_GramProBack[1][tempstr2].pKn;
      else
      {
        if (m_GramProBack[0].count(gramVec[1]) > 0 && m_GramProBack[0].count(gramVec[2]) > 0)//w2 and w3 are both in the vocabulary
          return m_GramProBack[0][gramVec[1]].bow + m_GramProBack[0][gramVec[2]].pKn;
        else
          return MIN_PRO;
      }

    }
    else
    {
      if (m_GramProBack[1].count(tempstr2) > 0)// the w2w3 gram exist
        return m_GramProBack[1][tempstr1].bow + m_GramProBack[1][tempstr2].pKn;
      else
      {
        if (m_GramProBack[0].count(gramVec[1]) > 0 && m_GramProBack[0].count(gramVec[2]) > 0)//w2 and w3 are both in the vocabulary
          return m_GramProBack[0][gramVec[1]].bow + m_GramProBack[0][gramVec[2]].pKn;
        else
          return MIN_PRO;
      }
    }
    return MIN_PRO; //if the words is unregistered ,give it the min prob MIN_PRO

  }
  /*
  * �����������Ĺ�ϵ��sentence���д��
  *socre the sentence with context information
  */
  double KnerserNeyBackOff::ScoreSentenceWithContext(vector<string> sentence)
  {
    vector<string> gramVec;
    double score = 0.0;
    int wordVecLen = sentence.size();
    int len_sent;
    len_sent = sentence.size() - N_GRAM;
    string singleGram;
    if (len_sent >= 0) //the num of wordVec is at least the same length with N_GRAM
    {
      int i = 0, j = 0;
      for (i = 0; i <= len_sent; i++)
      {
        gramVec.clear();
        for (j = i; j < N_GRAM + i - 1; j++) //combine the words of N_GRAM to a string,which is splited with space 
        {
          gramVec.push_back(sentence[j]);

        }
        gramVec.push_back(sentence[j]);
        score += ScoreGram(gramVec);
      }
    }
    return score;
  }
  /*
  *��������������Ϣ��sentence���
  *score the sentence without context
  */
  double KnerserNeyBackOff::ScoreSentenceWithoutContext(vector<string> sentence)
  {
    vector<string> gramVec;
    double score = 0.0;
    int sentenceLen = sentence.size();
    int len_sent = sentenceLen - N_GRAM;
    if (m_GramProBack[0].count(sentence[0]) > 0)
      score += m_GramProBack[0][sentence[0]].pKn; // caculate the w1's probability
    else
      score += MIN_PRO;
    if (sentence.size() == 1){
      return score;
    }
    string temp;
    temp = sentence[0] + " " + sentence[1];
    if (m_GramProBack[1].count(temp) > 0)
      score += m_GramProBack[1][temp].pKn; //  caculate the w2|w1 probability
    else
      score += MIN_PRO;
    if (len_sent >= 0) // the num of wordVec is at least the same length with N_GRAM
    {
      int i = 0, j = 0;
      for (i = 0; i <= len_sent; i++)
      {
        gramVec.clear();
        for (j = i; j < N_GRAM + i - 1; j++) //combine the words of N_GRAM to a string,which is splited with space 
        {
          gramVec.push_back(sentence[j]);

        }
        gramVec.push_back(sentence[j]);
        score += ScoreGram(gramVec);
      }
    }
    return score;

  }
}
