#!/bin/sh

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/zhangjie/test/share_lib
#export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/zhangjie/test/share_lib/build
#ldd libsecond.so 
./main
