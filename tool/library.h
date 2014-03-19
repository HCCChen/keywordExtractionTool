/*
	Author: Paul Chen
	Date: 2014//
	Target: 
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <sys/types.h>
#include <dirent.h>
#include <scws.h>
#include <errno.h>

using namespace std;

int getdir(string dir, vector<string> &files);
int countFileLine(string filePath);

