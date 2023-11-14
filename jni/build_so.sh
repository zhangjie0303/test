#!/bin/sh

#g++ -shared -fPIC -o libhello.so -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux HelloWorld.c -lc
#gcc -shared -fPIC -o libhello.so -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux HelloWorld.c -lc
g++ -shared -fPIC -o libhello.so -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux HelloWorld.c 
