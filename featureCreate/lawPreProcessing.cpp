/*
	Date: 2013/01/07
	Author: Paul Chen
	Target: Add line information in each law file
*/

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int getdir(string dir, vector<string> &files){
		DIR *dp;//創立資料夾指標
		struct dirent *dirp;
		if((dp = opendir(dir.c_str())) == NULL){
				cout << "Error(" << errno << ") opening " << dir << endl;
				return errno;
		}
		while((dirp = readdir(dp)) != NULL){//如果dirent指標非空
				files.push_back(string(dirp->d_name));//將資料夾和檔案名放入vector
		}
		closedir(dp);//關閉資料夾指標
		return 0;
}

int main(int argc, char* argv[]){
		const string SOURCE_DIR_PATH = "data/preProcessLaw/";
		const string TARGET_DIR_PATH = "data/testLawBase/";
		const string LAW_INFO_SUM = "data/totalLawInfo";
		vector<string> files = vector<string>();	//Save each File Name
		char strbuf[4096];
		unsigned int lineCount, i, totalLine = 0;
		string fullPath, tmpStr;
		getdir(SOURCE_DIR_PATH, files);
		fstream fin, fout, fInfo;
		fInfo.open(LAW_INFO_SUM.c_str(), fstream::out);
		for(i=0, lineCount = 0; i<files.size(); i++, lineCount = 0){//For each File
				if(files[i].length() < 3){continue;}
				fullPath = SOURCE_DIR_PATH + files[i];
				fin.open(fullPath.c_str(), fstream::in);
				fullPath = TARGET_DIR_PATH + files[i];
				fout.open(fullPath.c_str(), fstream::out);
				totalLine = 0;
				while(!fin.eof()){//For each line
					fin.getline(strbuf, 4096);
					tmpStr.assign(strbuf);
					if(tmpStr.length() < 3){break;}
					//Artical analysis
					totalLine++;
					if(tmpStr.find("第 ") != string::npos && tmpStr.find("章 ") != string::npos){
						lineCount = 1;
					}
					else{
						lineCount++;
					}
					
					fout << lineCount << ',' << files[i] << ',' << strbuf << endl;
				}
				fInfo << files[i] << "," << totalLine << endl;
				fin.close();
				fout.close();
		}
		fInfo.close();
		return 0;
}
