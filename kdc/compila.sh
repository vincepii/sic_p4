#! /bin/bash
g++ -Wall ../include/utilities.cpp -c
g++ -Wall ../include/encryption.cpp -c
g++ -Wall ../include/comm_layer.cpp -c
g++ -Wall -lpthread -lssl kdc.cpp utilities.o encryption.o comm_layer.o -o kdc
#./kdc 2117 &
#sleep 2s
#./apeer localhost 2117 &
