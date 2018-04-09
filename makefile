CC = g++
FLAGS = -Wall -O2
CC += $(FLAGS)
CFLAGS = -IPicoSHA2 -Icpp-httplib

all: miner.o
	$(CC) blockproxi.o miner.o -o coinpy-cpp-miner

test: blockproxi_test
	./blockproxi_test

blockproxi.o: blockproxi.cpp PicoSHA2/picosha2
	$(CC) -c blockproxi.cpp $(CFLAGS)

miner.o: blockproxi.cpp miner.cpp
	$(CC) -c blockproxi.cpp miner.cpp $(CFLAGS)

blockproxi_test: blockproxi.cpp PicoSHA2/picosha2.h
	$(CC) blockproxi.cpp -o $@ $(CFLAGS) -DCOINPY_CPP_MINER_SELFTEST
