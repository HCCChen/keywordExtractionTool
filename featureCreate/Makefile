HADOOP_INSTALL = /usr/local/hadoop-0.20.2
INCLUDE_PATH = $(HADOOP_INSTALL)/src/c++/
PROJECT_PATH = /home/hsng/keyworkExtraction

CC = g++
CXXFLAGS = -Wall -g \
                  -I${INCLUDE_PATH}pipes/api/hadoop \
                  -I${INCLUDE_PATH}utils/api/hadoop \
                  -I${HADOOP_INSTALL}/c++/Linux-i386-32/include \
                  -L${HADOOP_INSTALL}/c++/Linux-i386-32/lib
ICTCLAS_FLAG = -L${PROJECT_PATH}/tool/ICTCLAS_API/libICTCLAS50.so \
               -O2 -std=c++0x -m32 -DOS_LINUX
LDFLAGS = -lhadooppipes -lhadooputils -lpthread -lcrypto

OBJECTS=wordcount.o
OBJECTS2=lawPreProcessing.o
OBJECTS3=chWordTaging.o ${PROJECT_PATH}/tool/ICTCLAS_API/ICTCLAS50.h

wordcount: $(OBJECTS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)
lawPreProcessing: $(OBJECTS2)
	$(CC) -o $@ $(OBJECTS2)
