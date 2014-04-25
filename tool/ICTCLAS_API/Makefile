CC = g++
CFLAGS = ../library.o ./libICTCLAS50.so -O2 -std=c++0x -m32 -DOS_LINUX
SCWSLINK = -lpthread -I/usr/local/scws/include/scws -L/usr/local/scws/lib -lscws -Wl,--rpath -Wl,/usr/local/scws/lib

main: dividWordByICTCLAS.cpp ICTCLAS50.h
	${CC} dividWordByICTCLAS.cpp ${CFLAGS} ${SCWSLINK} -o dividWordByICTCLAS.o
addWordTag: addWordTag.cpp ICTCLAS50.h
	${CC} addWordTag.cpp ${CFLAGS} ${SCWSLINK} -o addWordTag.o
clear:
	rm *.o
