CC=g++
SRC=./src
INC=./include
CFLAGS=-std=c++17 -I./include

market-replayer: market-replayer.o
	$(CC) -o market-replayer market-replayer.o $(CFLAGS)

market-replayer.o: $(INC)/messagetype.h $(INC)/datareader.h $(SRC)/market_replayer.cc
	$(CC) -c -o market-replayer.o $(SRC)/market_replayer.cc -I$(INC) $(CFLAGS)

clean:
	rm -f *.o
	rm market-replayer
