#! /bin/bash

g++ -Wall ../../include/utilities.cpp -c
g++ -Wall ../../include/asym_enc.cpp -c
g++ -Wall -lssl main.cpp utilities.o asym_enc.o -o main
