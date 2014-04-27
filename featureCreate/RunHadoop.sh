#!/bin/bash
# Program:
#       Test Hadoop
# History:
# 2013/12/30	VBird	First release

printf "\e[1;33;40mInitialize HDFS\e[0m\n"
hadoop fs -rmr output
hadoop fs -rm /user/hsng/bin/wordcount
hadoop fs -rm /user/hsng/inputFile/*
printf "\e[1;33;40mUpload Map-Reduce Program\e[0m\n"
hadoop fs -put wordcount /user/hsng/bin/
printf "\e[1;33;40mUpload Map-Reduce Data\e[0m\n"
hadoop fs -put data/testLawBase/* /user/hsng/inputFile/

printf "\e[1;33;40m===========For Training Positive Data===========\e[0m\n"
cp data/trainingPosKeyword data/testingKeyword
cd ../tool/ICTCLAS_API/
./addWordTag.cpp
cd ../../featureCreate/

printf "\e[1;33;40mExecute Map-Reduce!\e[0m\n"
hadoop pipes -D hadoop.pipes.java.recordreader=true -D hadoop.pipes.java.recordwriter=true -D mapred.reduce.tasks=2 -files data/testingKeywordFix,data/languageBase/specialWordLib,data/languageBase/languageRule,data/totalLawInfo -input /user/hsng/inputFile/ -output /user/hsng/output -program /user/hsng/bin/wordcount
printf "\e[1;33;40mOutput Result File to 'trainingPosBase'\e[0m\n"
hadoop fs -cat output/part-* > trainingPosBase

printf "\e[1;33;40m===========For Training Negative Data===========\e[0m\n"
hadoop fs -rmr output
cp data/trainingNegKeyword data/testingKeyword
cd ../tool/ICTCLAS_API/
./addWordTag.cpp
cd ../../featureCreate/

printf "\e[1;33;40mExecute Map-Reduce!\e[0m\n"
hadoop pipes -D hadoop.pipes.java.recordreader=true -D hadoop.pipes.java.recordwriter=true -D mapred.reduce.tasks=2 -files data/testingKeywordFix,data/languageBase/specialWordLib,data/languageBase/languageRule,data/totalLawInfo -input /user/hsng/inputFile/ -output /user/hsng/output -program /user/hsng/bin/wordcount
printf "\e[1;33;40mOutput Result File to 'trainingNegBase'\e[0m\n"
hadoop fs -cat output/part-* > trainingNegBase
