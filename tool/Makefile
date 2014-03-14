CC = g++
CFLAGS = -O2 -std=c++0x

all: decodeSVMResult.cpp getNegKeyword.cpp svmPreprocess.cpp
	${CC} evaluateResult.cpp ${CFLAGS} -o evaluateResult.out
	${CC} decodeSVMResult.cpp ${CFLAGS} -o decodeSVMResult.out
	${CC} getNegKeyword.cpp -lpthread -I/usr/local/scws/include/scws -L/usr/local/scws/lib -lscws -Wl,--rpath -Wl,/usr/local/scws/lib ${CFLAGS} -o ./getNegKeyword.out
	${CC} svmPreprocess.cpp ${CFLAGS} -o svmPreprocess.out
decodeSVMResult.out: decodeSVMResult.cpp
	${CC} decodeSVMResult.cpp ${CFLAGS} -o decodeSVMResult.out
getNegKeyword.out: getNegKeyword.cpp
	${CC} getNegKeyword.cpp -lpthread -I/usr/local/scws/include/scws -L/usr/local/scws/lib -lscws -Wl,--rpath -Wl,/usr/local/scws/lib ${CFLAGS} -o ./getNegKeyword.out
svmPreprocess.out: svmPreprocess.cpp
	${CC} svmPreprocess.cpp ${CFLAGS} -o svmPreprocess.out
evaluateResult.out: evaluateResult.cpp
	${CC} evaluateResult.cpp ${CFLAGS} -o evaluateResult.out
clear:
	rm *.out
