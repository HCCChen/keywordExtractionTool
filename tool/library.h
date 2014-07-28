/*
	Author: Paul Chen
	Date: 2014//
	Target: 
*/
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iterator>
#include <algorithm>
#include <regex.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

using namespace std;

//Get all file name in director
int getdir(string dir, vector<string> &files);
//Count the total line in file
int countFileLine(string filePath);
//integer to string
string int2str(int &i);
//integer to string
int str2int(string str);
//Divide Sentence base on flag character
int explode(char divideChar, string originalString, vector<string> &stringAry);
//Replace all sub_string
string strReplaceAll(string sources, string originStr, string targetStr);
