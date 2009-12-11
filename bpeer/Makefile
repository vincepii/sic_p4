CC=g++
OPT=-lpthread -Wall -lssl
BIN=../bin/B
INCLUDE=../include
COMMON=$(INCLUDE)/utilities.h $(INCLUDE)/encryption.h $(INCLUDE)/asym_enc.h $(INCLUDE)/comm_layer.h
OBJ=$(INCLUDE)/utilities.o $(INCLUDE)/encryption.o $(INCLUDE)/asym_enc.o $(INCLUDE)/comm_layer.o

all: bpeer.o $(OBJ)
	$(CC) $(OPT) bpeer.o $(OBJ) -o $(BIN)/bpeer

bpeer.o: bpeer.cpp $(COMMON)
	$(CC) $(OPT) bpeer.cpp -c

$(INCLUDE)/utilities.o:
	cd $(INCLUDE) && make utilities.o
	
$(INCLUDE)/encryption.o:
	cd $(INCLUDE) && make encryption.o
	
$(INCLUDE)/asym_enc.o:
	cd $(INCLUDE) && make asym_enc.o

$(INCLUDE)/comm_layer.o:
	cd $(INCLUDE) && make comm_layer.o

clean:
	rm -f $(BIN)/bpeer *.o *~
