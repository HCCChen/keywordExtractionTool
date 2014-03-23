/*
	Author: Paul Chen
	Date: 2014/03/19
	Target: Divide word and get POS tag by ICTCLAS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include  <string>
#include "ICTCLAS50.h"
#include "../library.h"

using namespace std;

string divideSentence(string sentence);

int main(int argc, char* argv[])
{
    char* sResult;
	const string DIR_PATH = "../../featureCreate/data/preProcessLaw/";
	vector<string> files = vector<string>();
	vector<string> lawSeg;
	fstream fin, fout;
	char buf[4096];
	string tmpStr, filePath, lawSentence;
	int i,j,k, pos1, pos2;
	char posFlag = 'N';

	if(!ICTCLAS_Init())			  
	{	printf("Init fails\n");
		return -1;	}

	getdir(DIR_PATH, files);
	for(i = 0; i < files.size(); i++){//For each Law
		if(files[i].length() < 3){continue;}
		filePath = DIR_PATH + files[i];
		fin.open(filePath.c_str(), ios::in);
		while(!fin.eof()){
			lawSeg.clear();
			fin.getline(buf, 4096);
			if(strlen(buf) < 2){break;}
			lawSentence.assign(buf);
			lawSentence = divideSentence(lawSentence);
			cout << lawSentence << endl;
			//Divide and get pos-tsg
			explode(' ', lawSentence, lawSeg);
			for(j = 0, posFlag = 'N'; j < lawSeg.size(); j++){//For each word
				if(posFlag == 'N'){//POS-merge Begin
					if(lawSeg[j].find("/m") != string::npos){
						posFlag = 'm';
						pos1 = j;
					}
					else if(lawSeg[j].find("/v") != string::npos){
						posFlag = 'v';
						pos1 = j;
					}
					else if(lawSeg[j].find("/a") != string::npos 
						|| lawSeg[j].find("/x") != string::npos){
						posFlag = 'a';
						pos1 = j;
						pos2 = j;
					}
					else if(lawSeg[j].find("/d") != string::npos){ 
						posFlag = 'd';
						pos1 = j;
						pos2 = j;
					}

				}
				else if(posFlag == 'm'){
					if(lawSeg[j].find("/m") != string::npos){//m+....m
						continue;
					}
					else if(lawSeg[j].find("/q") != string::npos || lawSeg[j].find("/f") != string::npos){//m+....+q
						//Merge it!!
						posFlag = 'N';
					}
					else{//Undefine Merge
						posFlag = 'N';
					}
				}
				else if(posFlag == 'v'){
					if(lawSeg[j].find("/n") != string::npos){//v+n
						//Merge it!!
						posFlag = 'N';
					}
					else{
						posFlag = 'N';
					}
				}
				else if(posFlag == 'a'){
					if(lawSeg[j].find("/n") != string::npos //a+n+...
					|| lawSeg[j].find("/vn") != string::npos //a+vn+...
					|| lawSeg[j].find("/a") != string::npos){//a+a+...
						pos2 = j;
						continue;
					}
					else{
						if(pos1 != pos2){
							//Merge it!!
							cout << "\t";
							for(k = pos1; k < j; k++){
								cout << lawSeg[k].substr(0, lawSeg[k].find('/'));
							}
							cout << endl;
							posFlag = 'N';	
						}
						posFlag = 'N';
					}
				}
				else if(posFlag == 'd'){
					if(lawSeg[j].find("/a") != string::npos //d+a
					|| lawSeg[j].find("/n") != string::npos //d+n
					|| lawSeg[j].find("/x") != string::npos){//d+x
						pos2 = j;
						continue;
					}
					else{
						if(lawSeg[j].find("/n") != string::npos//d+...+n
						|| lawSeg[j].find("/vn") != string::npos//d+...+vn
						|| lawSeg[j].find("/q") != string::npos){pos2++;}//d+...+q
						if(pos1 != pos2){
							cout << "\t";
							for(k = pos1; k < j; k++){
								cout << lawSeg[k].substr(0, lawSeg[k].find('/'));
							}
							cout << endl;
							posFlag = 'N';	
						}
						posFlag = 'N';
					}
				}

			}
		}
		fin.close();
	}
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
