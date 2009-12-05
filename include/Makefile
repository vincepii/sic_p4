CC=g++
OPT=-Wall -lssl

utilities.o: utilities.cpp utilities.h
	$(CC) $(OPT) utilities.cpp -c

comm_layer.o: comm_layer.cpp comm_layer.h utilities.cpp
	$(CC) $(OPT) comm_layer.cpp -c

encryption.o: encryption.cpp encryption.h utilities.cpp
	$(CC) $(OPT) encryption.cpp -c
	
asym_enc.o: asym_enc.cpp asym_enc.h utilities.cpp
	$(CC) $(OPT) asym_enc.cpp -c

clean:
	rm -f *.o *~