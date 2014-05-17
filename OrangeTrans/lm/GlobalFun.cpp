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

#include "GlobalFun.h"

namespace OrangeLM
{
  bool IsEndOfSentence(string words){
    string endIcon[ENDICON_NUM] = { "��", ".", "��", "��", "?", ";", "��", ",", "!", "��", "\"", "{", "}" };
    for (int i = 0; i < ENDICON_NUM; i++)
    {
      if (words.compare(endIcon[i]) == 0)
        return true;
    }
    return false;
  }
  int PowInt(int num1, int index)
  {
    int result = 1;
    for (int i = 0; i < index; i++)
    {
      result *= num1;
    }
    return result;
  }
  void FilterString(string &str)
  {
    string::size_type pos = 0;
    /*string filt_elems[5] ={"\"",":"," ","��",""};
    int len = 5;
    for(int i=0;i<len;i++)
    {
    size_t len_elems = filt_elems[i].size();
    while (( pos = str.find( filt_elems[i], pos ))!= string::npos)
    {
    str.erase( pos, len_elems);
    }
    }*/
    string sep = " ";
    while ((pos = str.find(sep, pos)) != string::npos)
    {
      str.erase(pos, 1);
    }

  }
  //��ָ��gramList��Ƶд��ָ���ļ�·��gramFreqSavePath��
  void WriteMapToFile(const string &gramFreqSavePath, GramListMap &gramList){
    ofstream fout;
    GramListMap::iterator itr = gramList.begin();
    GramListMap::iterator itr_end = gramList.end();
    fout.open(gramFreqSavePath);
    for (; itr != itr_end; itr++)
    {
      fout << itr->first << "\t" << itr->second << endl; //���Ʊ�� ��ʽ ����Ƶ�� gram�ָ�
    }
    fout.close();
    //return true;
  }
  //��ָ��gramList����д��ָ���ļ�·��gramFreqSavePath��
  void WriteProbToFile(const string &gramFreqSavePath, GramProb &gramList){
    ofstream fout;
    GramProb::iterator itr = gramList.begin();
    GramProb::iterator itr_end = gramList.end();
    fout.open(gramFreqSavePath);
    if (!fout)
    {
      cout << "Failed to open File :" << gramFreqSavePath << endl;
      return;
    }
    for (; itr != itr_end; itr++)
    {
      fout << itr->first << "\t" << itr->second << endl; //���Ʊ�� ��ʽ ����Ƶ�� gram�ָ�
    }
    fout.close();
    //return true;
  }
  //��ָ��gramList����д��ָ���ļ�·��gramFreqSavePathĿ¼���µ������ļ���
  void WriteProbSplitBowToFile(const string &gramFreqSavePath, vector<GramProbBow> &gramList){
    ofstream fout;
    string path;
    string gramNameList[N_GRAM] = { "unigram.txt", "bigram.txt", "trigram.txt" };
    for (int j = 0; j < N_GRAM; j++)
    {
      path = gramFreqSavePath + gramNameList[j];
      fout.open(path);
      cout << "begin to write the proMap to file..." << endl;
      if (!fout)
      {
        cout << "Failed to open File :" << gramFreqSavePath << endl;
        return;
      }

      GramProbBow::iterator itr;
      GramProbBow::iterator itr_end;
      itr_end = gramList[j].end();
      itr = gramList[j].begin();
      for (; itr != itr_end; itr++)
      {
        if (itr->second.bow == 0.0)
          fout << itr->second.pKn << "\t" << itr->first << endl; //���Ʊ�� ��ʽ ����Ƶ�� gram�ָ�
        else
          fout << itr->second.pKn << "\t" << itr->first << "\t" << itr->second.bow << endl;
      }
      fout.close();
      cout << "successfully write the " << j + 1 << "-gram prob to file!" << endl;


    }
    cout << "finished to write the proMap to file!" << endl;
    //return true;
  }
  //��ָ��gramList����д��ָ���ļ�·��gramFreqSavePath��
  void WriteProbBowToFile(const string &gramFreqSavePath, vector<GramProbBow> &gramList){
    ofstream fout;
    string path = gramFreqSavePath + "kn_3gram_pro.txt";
    fout.open(path);
    cout << "begin to write the proMap to file..." << endl;
    if (!fout)
    {
      cout << "Failed to open File :" << gramFreqSavePath << endl;
      return;
    }
    fout << endl << "\\data\\" << endl;
    for (int i = 0; i < N_GRAM; i++)
    {
      fout << "ngram " << i + 1 << "=" << gramList[i].size() << endl;;
    }
    fout << endl;
    GramProbBow::iterator itr;
    GramProbBow::iterator itr_end;
    for (int i = 0; i < N_GRAM; i++)
    {
      fout << "\\" << i + 1 << "-grams:" << endl;
      itr_end = gramList[i].end();
      itr = gramList[i].begin();
      for (; itr != itr_end; itr++)
      {
        if (itr->second.bow == 0.0)
          fout << itr->second.pKn << "\t" << itr->first << endl; //���Ʊ�� ��ʽ ����Ƶ�� gram�ָ�
        else
          fout << itr->second.pKn << "\t" << itr->first << "\t" << itr->second.bow << endl;
      }
      fout << endl;
      cout << "successfully write the " << i + 1 << "-gram prob to file!" << endl;
    }

    fout << "\\end\\" << endl;
    fout.close();
    cout << "finished to write the proMap to file!" << endl;
    //return true;
  }

  //��ָ��gramList����д��ָ���ļ�·��gramFreqSavePath��
  void WriteProbVectorToFile(const string &gramFreqSavePath, vector<GramProb> &gramList){
    ofstream fout;
    fout.open(gramFreqSavePath);
    if (!fout)
    {
      cout << "Failed to open File :" << gramFreqSavePath << endl;
      return;
    }
    fout << endl << "\\data\\" << endl;
    for (int i = 0; i < N_GRAM; i++)
    {
      fout << "ngram " << i + 1 << "=" << gramList[i].size() << endl;;
    }
    fout << endl;
    GramProb::iterator itr;
    GramProb::iterator itr_end;
    for (int i = 0; i < N_GRAM; i++)
    {
      fout << "\\2-grams:" << endl;
      itr_end = gramList[i].end();
      itr = gramList[i].begin();
      for (; itr != itr_end; itr++)
      {
        fout << itr->second << "\t" << itr->first << endl; //���Ʊ�� ��ʽ ����Ƶ�� gram�ָ�
      }
      fout << endl;
    }

    fout << "\\end\\" << endl;
    fout.close();
    //return true;
  }
  /*
  *��ȡָ��·���µ�gram��Ƶ
  */
  void GetGramFreq(const string &gramSavePath, GramListMap &gramList)
  {
    ifstream fin;
    //GramListMap gramList;
    fin.open(gramSavePath);
    if (!fin)
    {
      cout << " file read error!" << endl;
      exit(0);
    }
    string text, tempstr1, tempstr2;
    char buf[1024];
    int count = 0;
    while (!fin.eof())
    {
      fin.getline(buf, 1024);
      text = buf;
      int pos = 0;
      int last_pos = 0;
      count++;
      if (count % 10000 == 0)
        cout << "count:" << count << endl;
      pos = text.find_first_of('\t', last_pos);
      tempstr1 = text.substr(last_pos, pos);
      tempstr2 = text.substr(pos + 1, string::npos - 1);
      if (!tempstr1.empty())
        gramList[tempstr1] = atoi(tempstr2.c_str());
    }
    fin.close();
    return;
  }
  /*
  *��ȡָ��·���µ�gram����
  */
  void GetGramPro(const string &gramSavePath, GramProbBow &gramList)
  {
    ifstream fin;
    //GramProb gramList;
    fin.open(gramSavePath);
    if (!fin)
    {
      cout << " file read error!" << endl;
      exit(0);
    }
    string text, tempstr1, tempstr2;
    char buf[1024];
    int count = 0;
    int count_t = 0;
    while (!fin.eof())
    {
      fin.getline(buf, 1024);
      text = buf;
      int pos = 0;
      int last_pos = 0;
      count++;
      if (count % 1000000 == 0)
        cout << "count:" << count << endl;
      count_t = 0;
      double pKn = 0.0;
      double bow = 0.0;
      while (true)
      {
        pos = text.find_first_of('\t', last_pos);
        if (pos >= string::npos)
          break;
        count_t++;
        tempstr1 = text.substr(last_pos, pos - last_pos);
        if (count_t == 1)
          pKn = atof(tempstr1.c_str());
        else if (count_t == 2)
          gramList[tempstr1].pKn = pKn;
        last_pos = pos + 1;
      }
      tempstr2 = text.substr(last_pos, text.size() - last_pos - 1);
      if (count_t == 1)
      {

        gramList[tempstr2].pKn = pKn;
        gramList[tempstr2].bow = 0.0;
      }
      else if (count_t == 2)
      {
        gramList[tempstr1].bow = atof(tempstr2.c_str());

      }
    }
    fin.close();
    //return gramList;
    return;
  }
  /*
  *��ָ����gramSrcMap���䵱ǰΪsrcLevelGram���͵�desLevelGram��������gramDesMap��
  */
  void  ReduceGramLevel(GramListMap &gramSrcMap, int srcLevel, GramListMap &gramDesMap, int desLevel)
  {
    GramListMap::iterator itr_src = gramSrcMap.begin();
    GramListMap::iterator itr_src_end = gramSrcMap.end();
    string gram, tempstr;
    int gramFreq = 0;
    string sep = " ";
    int sublen = srcLevel - desLevel;
    for (; itr_src != itr_src_end; itr_src++)
    {
      gram = itr_src->first;
      gramFreq = itr_src->second;
      int last_pos = 0;
      int pos = 0;
      for (int i = 0; i < sublen; i++) //��ȡ ָ����gram
      {
        pos = gram.find_first_of(sep, last_pos);
        last_pos = pos + 1;
      }
      tempstr = gram.substr(last_pos, string::npos - 1);
      gramDesMap[tempstr] += gramFreq;
    }

  }
  /*
  *��������ľ��ӣ�����ָ���Ϊvector
  *sentenceΪ����ľ��ӣ�wordVectorΪ����ĵ���������flagΪ��־λ Ϊ0 ��ʾ Ӣ�ģ�1��ʾ����
  */
  void SentenceToVector(string sentence, vector<string> &wordVector, int flag)
  {
    int last_pos = 0, pos = 0;
    string tempstr;
    string sep = " ";
    wordVector.push_back("<s>");
    while (true)
    {
      pos = sentence.find(sep, last_pos);
      if (pos == string::npos)
      {
        break;
      }
      tempstr = sentence.substr(last_pos, pos - last_pos);
      wordVector.push_back(tempstr);
      last_pos = pos + 1;
    }
    wordVector.push_back("</s>");

  }


  //����������ַ���������ָ�Ϊ����vector

  void StringToVector(string str, vector<string>&wordVector)
  {
    int pos = 0, last_pos = 0;
    string temp_str;
    while (true)
    {
      pos = str.find_first_of(" ", last_pos);

      if (pos >= string::npos)
        break;
      temp_str = str.substr(last_pos, pos - last_pos);

      wordVector.push_back(temp_str);

      last_pos = pos + 1;
    }
  }
}
