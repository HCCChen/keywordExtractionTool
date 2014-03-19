/*
	Author: Paul Chen
	Date: 2014/03/04
	Target: All-Yes = NO
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <sys/types.h>
#include <dirent.h>
#include <scws.h>
#include "library.h"

using namespace std;

int main(int argc, char* argv[]){
	const string LAW_DIR_PATH = "../featureCreate/data/preProcessLaw";
	const string NEGWORD_PATH = "../featureCreate/data/trainingNegKeyword";
	string filePath, tmpString, divideWord;
	fstream fin, fout;
	vector<string> 	files = vector<string>();
	map<string, string> 	knownWord;
	map<string, string>::iterator iter;
	int i;

	scws_t s;
	scws_res_t res, cur;
	char text[65536];
	char resultStr[512];

	if (!(s = scws_new())) {
		printf("ERROR: cann't init the scws!\n");
		exit(-1);
	}
	scws_set_charset(s, "utf8");
	scws_set_dict(s, "/usr/local/scws/etc/dict_cht.utf8.xdb", SCWS_XDICT_XDB);
	scws_set_rule(s, "/usr/local/scws/etc/rules_cht.utf8.ini");
	scws_set_duality(s, 1);
	scws_set_multi(s, SCWS_MULTI_ZALL);

	fout.open(NEGWORD_PATH.c_str(), ios::out);
	getdir(LAW_DIR_PATH, files);
	for(i=0; i<files.size(); i++){//For each File
		if(files[i] == "." || files[i] == ".."){continue;}
		//Load File
		filePath = LAW_DIR_PATH + "/" + files[i];
		fin.open(filePath.c_str(), ios::in);
		while(!fin.eof()){
			fin.getline(text, 4096);
			scws_send_text(s, text, strlen(text));
			while (res = cur = scws_get_result(s))
			{
				while (cur != NULL)
				{
					sprintf(resultStr, "WORD: %.*s/%s (IDF = %4.2f)\n", cur->len, text+cur->off, cur->attr, cur->idf);
					tmpString.assign(resultStr);
					divideWord = tmpString.substr(tmpString.find(' ')+1, tmpString.find("/")-tmpString.find(' ')-1);

					if(divideWord.length() > 3){
						iter = knownWord.find(divideWord);
						if(iter == knownWord.end()){
							knownWord[divideWord] = divideWord;
							fout << divideWord << endl;
						}
					}

					cur = cur->next;
				}
				scws_free_result(res);
			}


		}
		fin.close();
	}
	return 0;
}

