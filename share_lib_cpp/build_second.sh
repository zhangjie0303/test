#!/bin/sh

#g++ -std=c++11 second.cpp -shared -fPIC -o libsecond.so -L. -lfirst
g++ -std=c++11 second.cpp -shared -fPIC -o libsecond.so -L/home/zhangjie/test/share_lib -lfirst
