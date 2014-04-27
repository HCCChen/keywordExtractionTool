#include"Pipes.hh"
#include"TemplateFactory.hh"
#include"StringUtils.hh"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using namespace std;

class DataCountMap: public HadoopPipes::Mapper{
		public:
				//Constructor
				DataCountMap(HadoopPipes::TaskContext&context){
						char strBuf[128];
						string tmpStr;
						std::vector<std::string>rule;
						fstream fin;
						//Loading Basic information
						fin.open("testingKeywordFix", ios::in);
						baseLib.resize(15000);
						while(!fin.eof()){
								if(baseLib.size() == baseLib.capacity()){
										baseLib.resize(baseLib.capacity()*2);
								}
								fin.getline(strBuf, 128);
								baseLib.push_back(strBuf);
						}
						fin.close();
						//Loading Basic information
						fin.open("languageRule", ios::in);
						languageRuleCount = 0;
						languageRule.resize(128);
						while(!fin.eof()){
								fin.getline(strBuf, 128);
								if(strBuf[0] == '/' || strBuf[0] == '*' || strBuf[0] == ' '){continue;}
								tmpStr.assign(strBuf);
								rule=HadoopUtils::splitString(tmpStr, ":");
								if(rule[0].length() > 2){
									languageRule[languageRuleCount++] = rule[0];
								}
						}
						fin.close();
				}
				//For each Sentence(Context)
				void map(HadoopPipes::MapContext&context){
						unsigned int loopCount = 0, loopCount2 = 0, appearCount = 0;
						size_t pos;
						string sentence = context.getInputValue(), sentenceBuf;
						string subFeatureSet, word, posTag;
						std::vector<std::string>lawInfo=HadoopUtils::splitString(context.getInputValue(), ",");
						for(loopCount = 0; loopCount < baseLib.size()-1; loopCount++){//For each word
								if(baseLib[loopCount] == "" && baseLib[loopCount].find("/") == string::npos){continue;}
								//context.emit(baseLib[loopCount], "SENTENCE_LENGTH:"+lawInfo[2]+":"+HadoopUtils::toString(sentence.length()/3));
								word = baseLib[loopCount].substr(0,baseLib[loopCount].find("/"));
								posTag = baseLib[loopCount].substr(baseLib[loopCount].find("/")+1);
								if(sentence.find(word) != string::npos){//Find it!!
									appearCount = 0;
									//Counter Appear Time
									sentenceBuf = sentence;
									pos = sentence.find(word);
									while(pos != string::npos){
										appearCount++;
										sentenceBuf = sentenceBuf.substr(pos+3, sentenceBuf.length());
										pos = sentenceBuf.find(word);
									}
									subFeatureSet = "APPEAR";
									subFeatureSet += ";LINENUM:"+lawInfo[0];
									subFeatureSet += ";APPEAR_TIMES:"+lawInfo[1]+":"+HadoopUtils::toString(appearCount);
									if(lawInfo[0] == "1"){
										subFeatureSet += ";IS_TITLE";
									}
									//Location
									subFeatureSet += ";POSITION:"+HadoopUtils::toString(sentence.find(word)/3);
									if(sentence.find(word) <= sentence.length()/3){
									//if(sentence.find(word) <= 15){
										subFeatureSet += ";LOCATE_HEAD";
									}
									else if(sentence.find(word) > (sentence.length()*2)/3){
									//else if(sentence.find(word) > sentence.length()-15){
										subFeatureSet += ";LOCATE_TAIL";
									}
									//Pos Tag
									if(posTag.find("n") != string::npos){subFeatureSet += ";POS:0";	}
									else if(posTag.find("v") != string::npos){subFeatureSet += ";POS:1";}
									else if(posTag.find("a") != string::npos){subFeatureSet += ";POS:2";}
									else if(posTag.find("d") != string::npos){subFeatureSet += ";POS:3";}
									else if(posTag.find("m") != string::npos){subFeatureSet += ";POS:4";}
									else if(posTag.find("f") != string::npos){subFeatureSet += ";POS:5";}
									else if(posTag.find("ph") != string::npos){subFeatureSet += ";POS:6";}
									else{subFeatureSet += ";POS:6";}
									//Gramma
									for(loopCount2 = 0; loopCount2 < languageRuleCount; loopCount2++){//For each Gramma
										if(sentence.find(word+languageRule[loopCount2]) != string::npos
										|| sentence.find(languageRule[loopCount2] + word) != string::npos){
											//context.emit(word, HadoopUtils::toString(loopCount2));
											subFeatureSet += ";" + languageRule[loopCount2];
										}
									}

									//context.emit(baseLib[loopCount], subFeatureSet);
									context.emit(word, subFeatureSet);
								}
						}
				}
		private:
				vector<string> baseLib;
				vector<string> languageRule;
				unsigned int languageRuleCount;
};
class DataCountReduce:public HadoopPipes::Reducer{
		public:
				//constructor
				DataCountReduce(HadoopPipes::TaskContext&context){
						char strBuf[128];
						fstream fin;
						string tmpStr;
						std::vector<std::string>rule;
						//Loading Basic information
						fin.open("specialWordLib", ios::in);
						specialLibCount = 0;
						specialLib.resize(128);
						while(!fin.eof()){
								fin.getline(strBuf, 64);
								specialLib[specialLibCount++].assign(strBuf);
						}
						fin.close();
						//Loading Basic information
						fin.open("languageRule", ios::in);
						languageRuleCount = 0;
						languageRule.resize(128);
						while(!fin.eof()){
								fin.getline(strBuf, 128);
								if(strBuf[0] == '/' || strBuf[0] == '*' || strBuf[0] == ' '){continue;}
								tmpStr.assign(strBuf);
								rule=HadoopUtils::splitString(tmpStr, ":");
								if(rule[0].length() > 2){
									languageRule[languageRuleCount++] = rule[0];
								}
						}
						fin.close();
						//Loading Basic information
						fin.open("totalLawInfo", ios::in);
						totalLawCount = 0;
						while(!fin.eof()){
								fin.getline(strBuf, 128);
								tmpStr.assign(strBuf);
								rule=HadoopUtils::splitString(tmpStr, ",");
								if(rule[0].length() > 2){
									totalLawCount++;
									lawLength[rule[0]] = HadoopUtils::toInt(rule[1]);
								}
						}
						fin.close();

				}
				//Reduce, For each key(word)
				void reduce(HadoopPipes::ReduceContext&context)
				{
					string feature, keyword = context.getInputKey();
					vector<string>featureInfo;
					vector<string>valueBuf;
					string featurePosition = "0", featureIsTitle = "0", featureInChapter = "65535";
					string featureLocateHead = "0", featureLocateTail = "0", featureGramma[128], featurePOSTag = "0";
					string mapIdx;
					int featureAppearLine=0, featureIdx, featureFrequency = 0;
					unsigned int loopCount = 0;
					featureIdx = 0;
					map<string, int> appearTimeInEachLaw;
					map<string, int>::iterator it;
					int appearLawCount = 0;
					double tfValue = 1, idfValue = 1;
					while(context.nextValue()){	//For each Value
							//解析Value值
							featureInfo = HadoopUtils::splitString(context.getInputValue(), ";");
							//context.emit(keyword,context.getInputValue());
							for(loopCount = 0; loopCount < featureInfo.size(); loopCount++){//For each feature
									valueBuf = HadoopUtils::splitString(featureInfo[loopCount], ":");
									if(valueBuf[0] == "APPEAR"){//Appear line
											featureAppearLine++;
									}
									else if(valueBuf[0] == "APPEAR_TIMES"){//Appear Times
											featureFrequency += HadoopUtils::toInt(valueBuf[2]);
											if(appearTimeInEachLaw.find(valueBuf[1]) == appearTimeInEachLaw.end()){
													appearTimeInEachLaw[valueBuf[1]] = HadoopUtils::toInt(valueBuf[2]);
													appearLawCount++;
											}
											else{
													//	appearTimeInEachLaw[valueBuf[1]] += HadoopUtils::toInt(valueBuf[2]);
											}
									}
									else if(valueBuf[0] == "POSITION"){//Appear Position in a Sentence
											if(HadoopUtils::toInt(featurePosition) > HadoopUtils::toInt(valueBuf[1])){
												featurePosition = valueBuf[1];
											}
									}
									else if(valueBuf[0] == "IS_TITLE"){//Is in the Title word
											featureIsTitle = "1";
									}
									else if(valueBuf[0] == "LINENUM"){//以篇章為單位，最早出現的行數
											if(HadoopUtils::toInt(valueBuf[1]) < HadoopUtils::toInt(featureInChapter))
													featureInChapter = valueBuf[1];
									}
									else if(valueBuf[0] == "LOCATE_HEAD"){//In the Sentence head?
											featureLocateHead = "1";
									}
									else if(valueBuf[0] == "LOCATE_TAIL"){//In the Sentence Tail
											featureLocateTail = "1";
									}
									else if(valueBuf[0] == "POS"){//POS tag Featrue
											featurePOSTag = valueBuf[1];
									}
									else if(valueBuf[0] != "NOT_APPEAR"){//For Gramma	
											for(loopCount = 0; loopCount < languageRuleCount; loopCount++){
													featureGramma[loopCount] = "0";
													if(languageRule[loopCount].find(valueBuf[0]) != string::npos){
															featureGramma[loopCount] = "1";
													}
											}
									}
							}
					}
					//TF-IDF
					idfValue = (double)appearLawCount/(double)totalLawCount;
					for(it = appearTimeInEachLaw.begin(), tfValue = 0; it != appearTimeInEachLaw.end(); ++it){
						mapIdx = it->first;
						tfValue += (double)appearTimeInEachLaw[mapIdx]/(double)lawLength[mapIdx];
					}
					//Output Each feature
					feature = "+1 1:"+HadoopUtils::toString(keyword.length()/3) + " ";
					feature += "2:"+HadoopUtils::toString(featureFrequency) + " ";
					feature += "3:"+HadoopUtils::toString(featureAppearLine) + " ";
					feature += "4:"+ConvertToString(tfValue*idfValue*1000) + " ";
					feature += "5:"+featurePOSTag + " ";
//					feature += "5:"+featurePosition + " ";

/*
					feature += "6:"+featureLocateHead + " ";
					feature += "7:"+featureLocateTail + " ";
					feature += "8:"+featureIsTitle + " ";
					feature += "9:"+featureInChapter + " ";
					//Feature Gramma
					for(loopCount = 0; loopCount < languageRuleCount; loopCount++){
						if(featureGramma[loopCount] == "1"){
							feature += HadoopUtils::toString(loopCount+10) + ":" + featureGramma[loopCount] + " ";
						}
					}
					//Feature: Special Word
					for(loopCount = 0, featureIdx = 10 + languageRuleCount; loopCount < specialLibCount; loopCount++, featureIdx++){
						if(specialLib[loopCount] != "" && keyword.find(specialLib[loopCount]) != string::npos){
							feature += HadoopUtils::toString(featureIdx) + ":" + HadoopUtils::toString(1) + " ";
						}
					}
*/
					context.emit(keyword,feature);
				}
				string ConvertToString(double value) {
					std::stringstream ss;
					ss << value;
					return ss.str();
				}
		private:
				vector<string> specialLib;
				vector<string> languageRule;
				map<string, int> lawLength;
				unsigned int languageRuleCount;
				unsigned int specialLibCount;
				int totalLawCount;
};
int  main( int  argc, char *argv[]){
		return  HadoopPipes::runTask(HadoopPipes::TemplateFactory<DataCountMap, DataCountReduce>());
}
