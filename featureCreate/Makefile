HADOOP_INSTALL = /usr/local/hadoop-0.20.2
INCLUDE_PATH = $(HADOOP_INSTALL)/src/c++/

CC = g++
CXXFLAGS = -Wall -g \
                  -I${INCLUDE_PATH}pipes/api/hadoop \
                  -I${INCLUDE_PATH}utils/api/hadoop \
                  -I${HADOOP_INSTALL}/c++/Linux-i386-32/include \
                  -L${HADOOP_INSTALL}/c++/Linux-i386-32/lib
LDFLAGS = -lhadooppipes -lhadooputils -lpthread -lcrypto

OBJECTS=wordcount.o
OBJECTS2=lawPreProcessing.o
wordcount: $(OBJECTS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)
lawPreProcessing: $(OBJECTS2)
	$(CC) -o $@ $(OBJECTS2)
