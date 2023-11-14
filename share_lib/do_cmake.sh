#!/bin/sh

rm -rf build
mkdir build
cd build

#export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/zhangjie/ClickHouse/use_so_lib/lib
#echo "LD PATH: ${LD_LIBRARY_PATH}"
#cmake -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_INSTALL_PREFIX=./ ..
cmake -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_INSTALL_PREFIX=./ ..

ninja
#ninja clickhouse
#ninja install

cd ..
