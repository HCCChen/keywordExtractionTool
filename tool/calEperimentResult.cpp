/*
	Author: Paul Chen
	Date: 2014/7/2
	Target: Calculate Experiment Reault: Precision rate and Recall rate
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "library.h"
using namespace std;

int main(int argc, char* argv[]){
	const string TEST_DATA_FILE = "../libsvm-3.17/testSet";
	const string PREDICT_RESULT_PATH = "../libsvm-3.17/predictResult";
	char buf[4096];
	string tmpStr;
	fstream fin, fout;
	int totalPos = 0, totalNeg = 0;
	int tp = 0,tn = 0, fp = 0, fn = 0;
	int i;
	fin.open(TEST_DATA_FILE.c_str(), ios::in);
	while(!fin.eof()){
		fin.getline(buf, 4096);
		tmpStr.assign(buf);
		if(tmpStr.length() < 2){continue;}
		if(tmpStr[0] == '+'){totalPos++;}
		else{totalNeg++;}
	}
	fin.close();

	fin.open(PREDICT_RESULT_PATH.c_str(), ios::in);
	i = 0;
	while(!fin.eof()){
		fin.getline(buf, 4096);
		tmpStr.assign(buf);
		if(i < totalPos){//True classify
			if(tmpStr == "1"){tp++;}
			else{fn++;}
		}
		else{//False classify
			if(tmpStr.find("-") == string::npos){fp++;}
			else{tn++;}
		}
		i++;
	}
	cout << ((double)tp/(double)(tp+fp)) << endl;
	cout << ((double)tp/(double)(tp+fn)) << endl;
	fin.close();

	return 0;
}
