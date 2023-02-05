CC=g++
SRC=./src
INC=./include
CFLAGS=-std=c++17 -I./include

replayer: replayer.o
	$(CC) -o replayer replayer.o $(CFLAGS)

replayer.o: $(INC)/messagetype.h $(INC)/datareader.h $(SRC)/replayer.cc
	$(CC) -c -o replayer.o $(SRC)/replayer.cc -I$(INC) $(CFLAGS)

clean:
	rm -f *.o
	rm replayer
