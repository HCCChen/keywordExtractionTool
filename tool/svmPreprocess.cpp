/*
	Author: Paul Chen
	Date: 2014/3/4
	Target: Preprocess for Feature File From Hadoop
*/
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]){
	const string POS_FILE_PATH = "../featureCreate/trainingPosBase";
	const string NEG_FILE_PATH = "../featureCreate/trainingNegBase";
	const string TRAIN_DATA_PATH = "../libsvm-3.17/trainingData";
	const string TRAIN_WORD_PATH = "../libsvm-3.17/trainingKeyword";
	fstream fin, fout1, fout2;
	char buf[4096];
	string tmpStr, rowData[2];

	//Ready to write File
	fout1.open(TRAIN_DATA_PATH.c_str(), ios::out);
	fout2.open(TRAIN_WORD_PATH.c_str(), ios::out);
	//Load file
	fin.open(POS_FILE_PATH.c_str(), ios::in);
	while(!fin.eof()){
		fin.getline(buf, 4096);
		tmpStr.assign(buf);
		rowData[0] = tmpStr.substr(0, tmpStr.find('\t'));
		rowData[1] = tmpStr.substr(tmpStr.find('\t')+1);
		if(rowData[0].length() < 3){continue;}
		fout1 << rowData[1] << endl;
		fout2 << rowData[0] << endl;
	}
	fin.close();	
	fin.open(NEG_FILE_PATH.c_str(), ios::in);
	while(!fin.eof()){
		fin.getline(buf, 4096);
		tmpStr.assign(buf);
		rowData[0] = tmpStr.substr(0, tmpStr.find('\t'));
		rowData[1] = tmpStr.substr(tmpStr.find('\t')+1);
		rowData[1][0] = '-';
		if(rowData[0].length() < 3){continue;}
		fout1 << rowData[1] << endl;
		fout2 << rowData[0] << endl;
	}

	fin.close();	
	fout1.close();
	fout2.close();
	
	return 0;
}
