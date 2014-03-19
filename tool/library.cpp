/*
	Author: Paul Chen
	Date: 2014//
	Target: 
*/
#include "library.h"

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

int countFileLine(string filePath){
	ifstream	fin;		//file point
	int 		count = 0;
	char 		ch[65536];		//Buffer
	fin.open(filePath.c_str());
	while(fin && !fin.eof()) { 
		fin.getline(ch, 65536);
		count++;
	}
	return count;
}

