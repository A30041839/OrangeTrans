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

#include "AbsoluteDiscounting.h"

namespace OrangeLM
{
  AbsoluteDiscounting::AbsoluteDiscounting(void)
  {
  }


  AbsoluteDiscounting::~AbsoluteDiscounting(void)
  {
  }
  /*
  *���þ��Լ�ֵ������ͽ�gram�ĸ���
  *gramMapListΪ�����n-gram��ͳ�ƴ�Ƶ
  *gramProbListΪ�����n-gramͳ�Ƹ���
  *allocatProΪ�������ǰgram���ܸ���
  *restProΪ�������ǰgram��ʣ��ĸ���
  */
  void AbsoluteDiscounting::UseAbsoluteDiscounting(GramListMap &gramMapList, int vocabularySize, string savePath)
  {
    string fileNameTri = savePath + PROB_FILENAME;
    string fileNameRest = savePath + RES_PROB_FILENAME;
    GramProb gramProbList;
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

      m_GramPro[itr_gram->first] = (((double)itr_gram->second - discountNum) / totalGramNum);
    }
    m_restPro = (1.0 - (double)(discountNum*gramListNum) / totalGramNum); //����Gramʣ�����
    m_restPro = m_restPro / PowInt(vocabularySize, N_GRAM);
    cout << "�������m_GramPro!" << endl;

    WriteProbToFile(fileNameTri, m_GramPro);

    ofstream fout;
    fout.open(fileNameRest);
    fout << m_restPro << endl;
    fout.close();
    cout << "�ɹ���m_gram�����ļ���" << endl;
    //return gramProbList;
  }
  /*
  *����ָ��·��gramProSavePath�µĸ���map ��gramList��
  */
  void AbsoluteDiscounting::LoadGramPro(const string gramProSavePath)
  {
    string prob_file = gramProSavePath + PROB_FILENAME;
    string rest_prob_file = gramProSavePath + RES_PROB_FILENAME;
    //GetGramPro(prob_file,m_GramPro);

    ifstream fin;
    fin.open(rest_prob_file);
    if (!fin)
    {
      cout << "Failed to open rest_prob file : " << rest_prob_file << endl;
      return;
    }
    char buf[1024];
    fin.getline(buf, 1024);
    //string rest_str = buf;
    m_restPro = atof(buf);
    fin.close();
    cout << "�ɹ�����gramPro�ļ�!" << endl;
  }
  /*
  * ���ھ���sentence���д��
  */
  double AbsoluteDiscounting::ScoreSentence(string sentence)
  {
    vector<string> wordVec;
    vector<string> gramVec;
    SentenceToVector(sentence, wordVec, 0);
    double score = 0.0;
    int wordVecLen = wordVec.size();
    int len_sent;
    len_sent = wordVec.size() - N_GRAM;
    string singleGram;
    if (len_sent >= 0) //���ӵ��������ٴ���N_GRAM����
    {
      gramVec.push_back(wordVec[0]);
      gramVec.push_back(wordVec[1]);
      score = ScoreGram(gramVec);
      int i = 0, j = 0;
      for (i = 1; i <= len_sent; i++)
      {
        gramVec.clear();
        for (j = i; j < N_GRAM + i - 1; j++) //��N_GRAM������ϳ�һ��string�������Կո�ָ�
        {
          gramVec.push_back(wordVec[j]);

        }
        gramVec.push_back(wordVec[j]);
        score *= ScoreGram(gramVec);
      }
    }
    return score;
  }
  double AbsoluteDiscounting::ScoreGram(vector<string> gramVec)//�������gram���д��
  {
    int gramSize = gramVec.size();
    string tempstr;
    int i = 0;
    for (i = 0; i<gramSize - 1; i++)
    {
      tempstr += gramVec[i];
      tempstr += " ";
    }
    tempstr += gramVec[i];
    if (m_GramPro.count(tempstr)>0)
      return m_GramPro[tempstr];
    else
      return m_restPro;
  }
}