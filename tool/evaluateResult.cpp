/*
	Author: Paul Chen
	Date: 2014/03/14
	Target: Summarize extractiuon result
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "library.h"
using namespace std;

int main(int argc, char* argv[]){
	const string SOURCE_DATA_PATH = "../libsvm-3.17/experimentRecord";
	const string RESULT_DATA_PATH = "../featureCreate/data/experimentResult";
	const string SOURCE_DATA_PATH2 = "../libsvm-3.17/statisticRecord";
	const string RESULT_DATA_PATH2 = "../featureCreate/data/statisticResult";
	char buf[4096];
	string tmpStr, strSeq;
	fstream fin, fout;
	int data1 = 0, data2 = 0, pos1, pos2, flag = 0;
	int lineFlag = 0, cycleFlag = 0;
	double accruacy, precision, recall, fmeasure;
	double sumOfAccuracy = 0, sumOfPrecision = 0, sumOfRecall = 0, sumOfFmeasure = 0;

	fin.open(SOURCE_DATA_PATH.c_str(), ios::in);
	fout.open(RESULT_DATA_PATH.c_str(), ios::out);
	
	while(!fin.eof()){
		fin.getline(buf, 4096);
		if(buf[0] == '='){
			flag++; 
			continue;
		}
		if(flag == 0){
			fout << buf << endl;
		}
		else{
			tmpStr.assign(buf);
			pos1 = tmpStr.find('(') + 1;
			pos2 = tmpStr.find_last_of('(')-2;
			strSeq = tmpStr.substr(pos1, pos2 - pos1);

			pos1 = strSeq.find('/');
			tmpStr = strSeq.substr(0, pos1);
			data1 += atoi(tmpStr.c_str());
			tmpStr = strSeq.substr(pos1+1);
			data2 += atoi(tmpStr.c_str());
		}
	}
	fout << "Accuracy = " <<((double)(data1)/(double)(data2))*100 << "% (" << data1 << "/" << data2 << ") (classification)" << endl;

	fin.close();
	fout.close();


	fin.open(SOURCE_DATA_PATH2.c_str(), ios::in);
	fout.open(RESULT_DATA_PATH2.c_str(), ios::out);
	lineFlag = 0;
	while(!fin.eof()){
		fin.getline(buf, 4096);
		tmpStr.assign(buf);
		if(lineFlag % 3 == 0){accruacy = atof(tmpStr.c_str());}
		else if (lineFlag % 3 == 1){precision = atof(tmpStr.c_str());}
		else{
			recall = atof(tmpStr.c_str());
			fmeasure = (2*precision*recall)/(precision+recall);

			sumOfRecall += recall;
			sumOfAccuracy += accruacy;
			sumOfPrecision += precision;
			sumOfFmeasure += fmeasure;
			cycleFlag++;
		}
		lineFlag++;
	}
	fout << "Accuracy: " << sumOfAccuracy/cycleFlag << endl;
	fout << "Precision: " << sumOfPrecision/cycleFlag << endl;
	fout << "Recall: " << sumOfRecall/cycleFlag << endl;
	fout << "F-measures: " << sumOfFmeasure/cycleFlag << endl;
	fin.close();
	fout.close();

	return 0;
}
