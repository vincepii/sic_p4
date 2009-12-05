CC=g++
OPT=-lpthread -Wall -lssl
BIN=../bin
INCLUDE=../include
COMMON=$(INCLUDE)/utilities.h $(INCLUDE)/encryption.h $(INCLUDE)/asym_enc.h $(INCLUDE)/comm_layer.h
OBJ=$(INCLUDE)/utilities.o $(INCLUDE)/encryption.o $(INCLUDE)/asym_enc.o $(INCLUDE)/comm_layer.o

all: apeer.o $(OBJ)
	$(CC) $(OPT) apeer.o $(OBJ) -o $(BIN)/apeer

apeer.o: apeer.cpp $(COMMON)
	$(CC) $(OPT) apeer.cpp -c

$(INCLUDE)/utilities.o:
	cd $(INCLUDE) && make utilities.o
	
$(INCLUDE)/encryption.o:
	cd $(INCLUDE) && make encryption.o
	
$(INCLUDE)/asym_enc.o:
	cd $(INCLUDE) && make asym_enc.o

$(INCLUDE)/comm_layer.o:
	cd $(INCLUDE) && make comm_layer.o

clean:
	rm -f $(BIN)/apeer *.o *~
