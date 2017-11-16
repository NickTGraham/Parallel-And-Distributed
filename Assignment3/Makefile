CC = gcc
#CC = /usr/local/bin/gcc-6
CR = g++
#CR = /usr/local/bin/g++-6
CCFLAGS = -fopenmp
CPPFLAGS = -std=c++11 -fcilkplus -lcilkrts
#SOURCE = $(wildcard *.cpp)
#OBJECTS = $(SOURCE:.cpp=.o)

default: all

all:
	$(CR) $(CPPFLAGS) silk.cpp -o silk $(LDFLAGS)
	$(CC) $(CCFLAGS) gauss.c -o gauss
	$(CC) $(CCFLAGS) openMP.c -o openMP

nop: silk.cpp gauss.c openMP.c
	$(CR) $(CPPFLAGS) -O0 silk.cpp -o silk $(LDFLAGS)
	$(CC) $(CCFLAGS) -O0 gauss.c -o gauss
	$(CC) $(CCFLAGS) -O0 openMP.c -o openMP

1op: silk.cpp gauss.c openMP.c
	$(CR) $(CPPFLAGS) -O1 silk.cpp -o silk $(LDFLAGS)
	$(CC) $(CCFLAGS) -O1 gauss.c -o gauss
	$(CC) $(CCFLAGS) -O1 openMP.c -o openMP

2op: silk.cpp gauss.c openMP.c
	$(CR) $(CPPFLAGS) -O2 silk.cpp -o silk $(LDFLAGS)
	$(CC) $(CCFLAGS) -O2 gauss.c -o gauss
	$(CC) $(CCFLAGS) -O2 openMP.c -o openMP

3op: silk.cpp gauss.c openMP.c
	$(CR) $(CPPFLAGS) -O3 silk.cpp -o silk $(LDFLAGS)
	$(CC) $(CCFLAGS) -O3 gauss.c -o gauss
	$(CC) $(CCFLAGS) -O3 openMP.c -o openMP
