#! /bin/bash
g++ -Wall ../../include/utilities.cpp -c
g++ -Wall ../../include/comm_layer.cpp -c
g++ -Wall ../../include/asym_enc.cpp -c
g++ -Wall -lpthread -lssl apeer.cpp utilities.o comm_layer.o -o apeer
g++ -Wall -lpthread -lssl kdc.cpp utilities.o comm_layer.o -o kdc
#./kdc 2020 &
#sleep 2s
#./apeer localhost 2020 &
