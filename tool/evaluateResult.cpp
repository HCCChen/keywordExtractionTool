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
using namespace std;

int main(int argc, char* argv[]){
	const string SOURCE_DATA_PATH = "../libsvm-3.17/experimentRecord";
	const string RESULT_DATA_PATH = "../featureCreate/data/experimentResult";
	char buf[4096];
	string tmpStr, strSeq;
	fstream fin, fout;
	int data1 = 0, data2 = 0, pos1, pos2, flag = 0;

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
	
	return 0;
}
