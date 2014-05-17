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

#ifndef GLOBAL_TRANS_PIPELINE_H_INCLUDED_
#define GLOBAL_TRANS_PIPELINE_H_INCLUDED_
#include<map>
#include<string>
#include<fstream>
#include<iostream>
#include<vector>
#include<stdlib.h>
#include <process.h> 
#include <direct.h>
#include <cmath>
#define N_GRAM 3 //��������ģ�Ͳ���trigram
#define ENDICON_NUM 13
#define UNIGRAM_FREQ_PATH  "unigram.txt"
#define MIN_PRO  -11111 //������С�ĸ��� ��δ��¼�Ĵ�
#define BIGRAM_FREQ_PATH  "bigram.txt"
#define TRIGRAM_FREQ_PATH "trigram.txt"
#define MAX_READ_LINE_LEN  1024 //���ÿ�ζ�ȡ����  

using namespace std;

namespace OrangeLM
{
  typedef struct{
    double pKn; //ƽ������
    double bow; //����Ȩ��
  }GramProbBack;
  typedef map<string, int> GramListMap;
  typedef map<string, double> GramProb;
  typedef map<string, GramProbBack> GramProbBow; //���к���Ȩ�ص�gram����map 
  /*
  *�ж϶��������words�Ƿ�Ϊ���ӵĽ�����־
  */
  bool IsEndOfSentence(string words);
  /*
  * ����num1��index�η�
  */
  int PowInt(int num1, int index);
  /*
  *�ַ�������
  */
  void FilterString(string &str);
  /*
  *��ȡָ��·���µ�gram��Ƶ
  */
  void GetGramFreq(const string &gramSavePath, GramListMap &gramList);
  /*
  *��ȡָ��·���µ�gram����
  */

  void GetGramPro(const string &gramSavePath, GramProbBow &gramList);
  /*
  *��ָ����gramSrcMap���䵱ǰΪsrcLevelGram���͵�desLevelGram��������gramDesMap��
  */
  void  ReduceGramLevel(GramListMap &gramSrcMap, int srcLevel, GramListMap &gramDesMap, int desLevel);

  //��ָ��gramListд��ָ���ļ�·��gramFreqSavePath��
  void  WriteMapToFile(const string &gramFreqSavePath, GramListMap &gramList);

  //��ָ��gramListд��ָ���ļ�·��gramFreqSavePath��
  void  WriteProbToFile(const string &gramFreqSavePath, GramProb &gramList);

  //��ָ����gramListVectorд���ļ� ����Ҫ������Ϊsrilm������ģ���ļ�
  void WriteProbVectorToFile(const string &gramFreqSavePath, vector<GramProb> &gramList);

  //��ָ���ĺ��л���Ȩ�ص�n-gram����mapд���ļ� ����Ҫ������Ϊsrilm������ģ���ļ�
  void WriteProbBowToFile(const string &gramFreqSavePath, vector<GramProbBow> &gramList);

  //��ָ��gramList����д��ָ���ļ�·��gramFreqSavePathĿ¼���µ������ļ���
  void WriteProbSplitBowToFile(const string &gramFreqSavePath, vector<GramProbBow> &gramList);

  //��������ľ��ӣ�����ָ���Ϊvector
  void  SentenceToVector(string sentence, vector<string> &wordVector, int flag);

  //����������ַ���������ָ�Ϊ����vector

  void StringToVector(string str, vector<string>&wordVector);
}

#endif