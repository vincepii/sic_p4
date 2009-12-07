#! /bin/bash
g++ -Wall ../../include/utilities.h ../../include/utilities.cpp -c
g++ -Wall ../../include/encryption.h ../../include/encryption.cpp -c
g++ -Wall ../../include/comm_layer.h ../../include/comm_layer.cpp -c
g++ -Wall -lpthread -lssl apeer.cpp utilities.o encryption.o comm_layer.o -o apeer
g++ -Wall -lpthread -lssl kdc.cpp utilities.o encryption.o comm_layer.o -o kdc
./kdc 2090 &
sleep 2s
./apeer localhost 2090 &
