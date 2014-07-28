/*
	Author: Paul Chen
	Date: 2014/03/19
	Target: Divide word and get POS tag by ICTCLAS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include "ICTCLAS50.h"
#include "../library.h"

using namespace std;

string divideSentence(string sentence);
string removeTag(string sentence);
bool filterOfChWord(string chWord);

int main(int argc, char* argv[])
{
	const string INPUT_PATH = "../../featureCreate/data/chBase";
	const string OUTPUT_PATH = "../../featureCreate/data/chWordCandidate";
	const string TEST_OUTPUT_PATH = "testResult";
	vector<string> lawSeg;
	map<string, int> wordPool; //<chWord, freq>
	map<string, int>::iterator iter;
	fstream fin, fout;
	char buf[4096];
	string tmpStr, title;
	map<string, int> wordSegLib;
	map<string, int>::iterator it;
	int i,j,k, pos1, pos2;

	if(!ICTCLAS_Init())			  
	{	printf("Init fails\n");
		return -1;	}

	unsigned int nItems=ICTCLAS_ImportUserDictFile("Data/userdict.txt",CODE_TYPE_UTF8);
	ICTCLAS_SaveTheUsrDic();
	cerr << nItems << " user-defined lexical entries added!" << endl;
	//Load File and Divide it
	fin.open(INPUT_PATH.c_str(), ios::in);
	fout.open(OUTPUT_PATH.c_str(), ios::out);
	while(!fin.eof()){//For each Law
		fin.getline(buf, 4096);
		tmpStr.assign(buf);
		//Special Case
		if(tmpStr.find("章　") != string::npos){//Chapter title
			title = tmpStr.substr(tmpStr.find("章　")+6);
			if(wordSegLib.find(title) == wordSegLib.end()){wordSegLib[title] = 1;}
			else{wordSegLib[title]++;}
		}
		if(tmpStr.find("章 ") != string::npos){//Chapter title
			title = tmpStr.substr(tmpStr.find("章 ")+4);
			if(wordSegLib.find(title) == wordSegLib.end()){wordSegLib[title] = 1;}
			else{wordSegLib[title]++;}
		}
		if(tmpStr.find("、") != string::npos && tmpStr.find("。") != string::npos){
			title = tmpStr.substr(tmpStr.find("、")+3, tmpStr.find("。")-tmpStr.find("、")-3);
			if(title.length() < 24 && title.find("或") == string::npos && title.find("及") == string::npos
			&& title.find("且") == string::npos && title.find("、") == string::npos){//Max to 8 word
				if(wordSegLib.find(title) == wordSegLib.end()){wordSegLib[title] = 1;}
				else{wordSegLib[title]++;}
			}
		}
		//Regualar case
		tmpStr = divideSentence(buf);
		tmpStr = removeTag(tmpStr);
		explode(' ', tmpStr, lawSeg);

		for(i = lawSeg.size()-1; i > 0; i--){//Merge Word
			if((lawSeg[i] == "元" || lawSeg[i] == "年" || lawSeg[i] == "條"  || lawSeg[i] == "項" || lawSeg[i] == "人" || lawSeg[i] == "日") && i != 0){
				lawSeg[i-1] = lawSeg[i-1] + lawSeg[i];
				lawSeg.erase(lawSeg.begin()+i);
			}
			else if((lawSeg[i] == "萬元" || lawSeg[i] == "萬人" || lawSeg[i] == "之人" || lawSeg[i] == "小時") && i != 0){
				lawSeg[i-1] = lawSeg[i-1] + lawSeg[i];
				lawSeg.erase(lawSeg.begin()+i);
			}
		}
		for(i = 0; i < lawSeg.size(); i++){//For each Word seg
			//Filter
			if(lawSeg[i].find("　") != string::npos || lawSeg[i].find("（") != string::npos 
			|| lawSeg[i].find("）") != string::npos || lawSeg[i].find("、") != string::npos
			|| lawSeg[i].find("「") != string::npos || lawSeg[i].find("」") != string::npos
			|| lawSeg[i].length() % 3 != 0){
				continue;
			}
			//Record it!
			if(wordSegLib.find(lawSeg[i]) == wordSegLib.end()){
				wordSegLib[lawSeg[i]] = 1;
			}
			else{
				wordSegLib[lawSeg[i]]++;
			}
		}
	}
	for(iter = wordSegLib.begin(); iter != wordSegLib.end(); iter++){
		tmpStr = iter->first;
		if(filterOfChWord(tmpStr) == true){
			fout << iter->first << endl;
		}
	}
	fin.close();
	fout.close();

	ICTCLAS_Exit();
	return 0;
}

string divideSentence(string sentence){
	int nPaLen=sentence.length();
	char* sRst=0;
	int nRstLen=0;
	string result;
	sRst=(char *)malloc(nPaLen*6);
	nRstLen=ICTCLAS_ParagraphProcess(sentence.c_str(),nPaLen,sRst ,CODE_TYPE_UTF8,1);
	result.assign(sRst);
	free(sRst);
	return result;
}


string removeTag(string sentence){
	int i, pos;
	string tmpStr, finalResult = "";
	vector<string> wordSeg;
	explode(' ', sentence, wordSeg);
	for(i = 0; i < wordSeg.size(); i++){
		tmpStr = wordSeg[i].substr(0, wordSeg[i].find('/'));
		finalResult += tmpStr + " ";
	}
	return finalResult;
}

bool filterOfChWord(string chWord){
	int i;
	if(chWord.length() < 6){return false;}
	if(chWord.find("，") != string::npos || chWord.find("。") != string::npos 
	|| chWord.find("；") != string::npos || chWord.find("：") != string::npos
	|| chWord.find('(') != string::npos || chWord.find(')') != string::npos){
		return false;
	}
	for(i = 0; i < chWord.length(); i++){
		if(chWord[i] >= '0' && chWord[i] <= '9'){return false;}
		if(chWord[i] == '(' && chWord[i] == ')'){return false;}
	}
	return true;
}
