#!/bin/bash

printf "\E[1;32;40mBegin Of Experiment! \E[0m\n"
printf "\E[0;33;40mFeature Creating \E[0m\n"
resultPath=experimentRecord
#create feature
cd featureCreate/
./RunHadoop.sh
#pre-process feature to libsvm
cd ../tool
./svmPreprocess.out
cd ../libsvm-3.17/tools/
python subset.py ../trainingData 14000 ../trainSet ../testSet
cd ../
#prepare Result File
#if [ -d $resultPath ]; then
#	rm $resultPath
#fi
#Experiment 1
printf "\E[0;33;40mExperiment1: train->test \E[0m\n"
./svm-train trainSet model
./svm-predict testSet model predictResult > $resultPath
#Experiment 2
printf "\E[0;33;40mExperiment2: test->train \E[0m\n"
./svm-train testSet model
./svm-predict trainSet model predictResult >> $resultPath
#Experiment 3
printf "\E[0;33;40mExperiment3: all->all \E[0m\n"
./svm-train trainingData model
./svm-predict trainingData model predictResult >> $resultPath

printf "\E[0;33;40mExperiment4: (train->test)*10 \E[0m\n"
echo "=======================" >> $resultPath
i=1
while [ $i -le 10 ] 
do 
 printf "\E[0;36;40m"
 echo -n "Times: $i" 
 printf "\E[0m\n"
 python ./tools/subset.py -s 1 trainingData 14000 trainSet testSet
 ./svm-train trainSet model
 ./svm-predict testSet model predictResult >> $resultPath
 i=`expr $i + 1` 
done 

printf "\E[0;33;40mConvert Result \E[0m\n"
cd ../tool
./evaluateResult.out
cd ../

printf "\E[1;32;40mEnd Of Experiment! \E[0m\n"
