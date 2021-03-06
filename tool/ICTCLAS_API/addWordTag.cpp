/*
	Author: Paul Chen
	Date: 2014/04/24
	Target: Add pos tag for each word by ICTCLAS
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
string outputMerge(int startPos, int endPos, vector<string> lawSeg, char posTag);

int main(int argc, char* argv[])
{
	//Initialize 
	//const string BASIC_WORD_PATH = "../../data/trainingNegKeyword";
	//const string OUTPUT_PATH = "../../data/trainingNegKeywordPosTag";
	const string BASIC_WORD_PATH = "../../featureCreate/data/testingKeyword";
	const string OUTPUT_PATH = "../../featureCreate/data/testingKeywordFix";
	fstream fin, fout;
	char buf[4096];
	string tmpStr, word, outputTag;
	vector<string> wordSeg;
	vector<string> tagPool;
	//index: tag
	//0: v; 1: n; 2: a; 3: d; 4: m
	int tagRecord[8];
	tagPool.resize(64);
	int i, flag;
	if(!ICTCLAS_Init()){
		printf("Init fails\n");
		return -1;
	}
	fin.open(BASIC_WORD_PATH.c_str(), ios::in);
	fout.open(OUTPUT_PATH.c_str(), ios::out);
	while(!fin.eof()){//For each word
		fin.getline(buf, 4096);
		tmpStr.assign(buf);
		if(tmpStr.length() < 3){continue;}
		tmpStr = divideSentence(tmpStr);
		for(i = 0; i < 8; i++){tagRecord[i] = 0;}
		outputTag = "n";
		//Analysis and Merge Pos tag
		wordSeg.clear();
		wordSeg.resize(0);
		explode(' ', tmpStr, wordSeg);
		for(i = 0, word = ""; i < wordSeg.size(); i++){//Analysis each word
			word += wordSeg[i].substr(0, wordSeg[i].find('/'));
			tagPool[i] = wordSeg[i].substr(wordSeg[i].find('/')+1);
			if(tagPool[i].find("v") != string::npos){tagRecord[0]++;}
			else if(tagPool[i].find("n") != string::npos){tagRecord[1]++;}
			else if(tagPool[i].find("a") != string::npos){tagRecord[2]++;}
			else if(tagPool[i].find("d") != string::npos){tagRecord[3]++;}
			else if(tagPool[i].find("m") != string::npos){tagRecord[4]++;}
		}
		flag = 2;//Find last pos.
		if(tagPool[wordSeg.size()-1].find("k") != string::npos){flag = 3;}

		//Add output tag
		if(wordSeg.size() == 2){//Only one tag
			outputTag = tagPool[0];
			//cout << tmpStr << " : " << outputTag << endl;
		}
		else if(wordSeg.size() > 2){//Find last tag
			if(tagPool[wordSeg.size()-flag].find("k") != string::npos){
				outputTag = "n";
			}
			else{
				outputTag = tagPool[wordSeg.size()-flag];
			}
		}//Only one tag
		fout << word << "/" << outputTag << endl;
	}
	fin.close();
	fout.close();
	ICTCLAS_Exit();
	return 0;
}

string divideSentence(string sentence){
	int nPaLen=sentence.length();
	char* sRst=0;
	sRst=(char*)malloc(nPaLen*6);
	int nRstLen=0;
	string result;
	nRstLen=ICTCLAS_ParagraphProcess(sentence.c_str(),nPaLen,sRst ,CODE_TYPE_UTF8,1);
	result.assign(sRst);
	free(sRst);
	if(result.find("/") == string::npos){
		result += "/";
		result += "n";
	}
	return result;
}


string outputMerge(int startPos, int endPos, vector<string> lawSeg, char posTag){
	int i,j;
	char tmp[1];
	tmp[0] = posTag;
	string result = "";
	string pos = "/";
	pos += posTag;
	for(i = startPos; i <= endPos; i++){
		result += lawSeg[i].substr(0, lawSeg[i].find('/'));
	}
	result += pos;
	return result;

}
