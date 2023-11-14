#!/bin/sh

rm -rf main
g++ -std=c++11 main.cpp -L. -lsecond -lfirst -o main
#g++ -std=c++11 main.cpp -L. -lfirst -lsecond -o main

#g++ -std=c++11 main.cpp -L. -lsecond -o main

#g++ -std=c++11 main.cpp -L./build -lsecond -lfirst -o main
#g++ -std=c++11 main.cpp -L./build -lsecond -o main

