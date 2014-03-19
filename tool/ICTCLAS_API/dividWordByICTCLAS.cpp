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

using namespace std;

string divideSentence(string sentence);

int main(int argc, char* argv[])
{
    char* sResult;
	if(!ICTCLAS_Init())			  
	{	printf("Init fails\n");
		return -1;	}
	if(argc == 2){
		string sParagraph = argv[1] ;
		cout << divideSentence(sParagraph) << endl;
		ICTCLAS_Exit();
	}
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
