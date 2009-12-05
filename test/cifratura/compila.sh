#! /bin/bash
g++ -Wall ../../include/utilities.h ../../include/utilities.cpp -c
g++ -Wall ../../include/comm_layer.h ../../include/comm_layer.cpp -c
g++ -Wall ../../include/encryption.h ../../include/encryption.cpp -c
g++ -Wall -lpthread -lssl peer.cpp utilities.o comm_layer.o encryption.o -o peer
#./peer &

