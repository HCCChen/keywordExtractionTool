#include "lib/analysis.h"
using namespace std;

int main(){
	//string lawPath = "./data/lawBase/";
	string lawPath = "../data/testLawBase/";
	string specialWordFile = "../data/languageBase/specialWordLib";
	string wordFormFile = "../data/languageBase/wordFormFile";

	string wordFileName = "../data/trainingPosKeyword";
	string outputFileName = "../data/trainingPosData";
	string wordFileName2 = "../data/trainingNegKeyword";
	string outputFileName2 = "../data/trainingNegData";
	string wordFileName3 = "../data/testingPosKeyword";
	string outputFileName3 = "../data/testingPosData";
	string wordFileName4 = "../data/testingNegKeyword";
	string outputFileName4 = "../data/testingNegData";
	string wordFileName5 = "../data/Result";
	string outputFileName5 = "../data/pcGetData";

	//效能分析
	time_t begin, end;
	double timeCnt;
	begin = time(NULL);


	cout << "初始化前置作業..." << endl;
	Analysis analysis(lawPath, specialWordFile, wordFormFile);
	cout << "從：" << wordFileName << "建立特徵值" <<endl;
	analysis.createScale(wordFileName, outputFileName, "+1");
	//輸出所需時間
	end = time(NULL);
	timeCnt = end - begin;
	cout << "所有特徵生成完成！耗時：" << timeCnt << "秒" << endl;

	return 0;
}
