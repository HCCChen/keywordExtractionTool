CC = g++
CFLAGS = library.o -Os -std=c++0x
SCWSLINK = -lpthread -I/usr/local/scws/include/scws -L/usr/local/scws/lib -lscws -Wl,--rpath -Wl,/usr/local/scws/lib

all: library
	${CC} createScaleFile.cpp lib/analysis.cpp -std=c++0x -lpthread -Os -o ./scaleCreating
	${CC} evaluateResult.cpp ${CFLAGS} -o evaluateResult.out
	${CC} decodeSVMResult.cpp ${CFLAGS} -o decodeSVMResult.out
	${CC} getNegKeyword.cpp ${SCWSLINK} ${CFLAGS} -o ./getNegKeyword.out
	${CC} svmPreprocess.cpp ${CFLAGS} -o svmPreprocess.out
	${CC} calEperimentResult.cpp  ${CFLAGS} -o calEperimentResult.out
library: library.cpp
	${CC} library.h library.cpp ${SCWSLINK} -c
createScaleFile: createScaleFile.cpp
	${CC} createScaleFile.cpp lib/analysis.cpp -std=c++0x -lpthread -Os -o ./scaleCreating
decodeSVMResult: decodeSVMResult.cpp
	${CC} decodeSVMResult.cpp ${CFLAGS} -o decodeSVMResult.out
getNegKeyword: getNegKeyword.cpp
	${CC} getNegKeyword.cpp ${SCWSLINK} ${CFLAGS} -o ./getNegKeyword.out
svmPreprocess: svmPreprocess.cpp
	${CC} svmPreprocess.cpp ${CFLAGS} -o svmPreprocess.out
evaluateResult: evaluateResult.cpp
	${CC} evaluateResult.cpp ${CFLAGS} -o evaluateResult.out
calEperimentResult: calEperimentResult.cpp
	${CC} calEperimentResult.cpp  ${CFLAGS} -o calEperimentResult.out
clear:
	rm *.out
	rm *.gch
	rm *.o
libraryTest: library.cpp
	${CC} library.h library.cpp ${SCWSLINK} -o library.out
