#!/bin/sh

rm -rf build
mkdir build

cd /opt/app/rocksdb
./clean.sh
cd -

cd build
ROCKS_LIB_DIR="/opt/app/version/rocksdb-7.7.3-cmake_build"


#以下的prefix路径需要指定安装gflags的prefix路径，否则编译过程中无法链接到gflags的库
#如果cmake 版本过低，使用cmake3
#DWITH_xxx 表示开启几个压缩算法的编译选项，否则运行db_bench时rocksdb产生数据压缩的时候无法找到对应的库
cmake .. -DCMAKE_INSTALL_PREFIX=${ROCKS_LIB_DIR} -DWITH_SNAPPY=1 -DWITH_LZ4=1 -DWITH_ZLIB=1 -DWITH_ZSTD=1 -DWITH_EXAMPLES=1 -DROCKSDB_BUILD_SHARED=0 -DCMAKE_BUILD_TYPE=Release
#cmake .. -DCMAKE_INSTALL_PREFIX=${ROCKS_LIB_DIR} -DWITH_SNAPPY=1 -DWITH_LZ4=1 -DWITH_ZLIB=1 -DWITH_ZSTD=1 -DWITH_EXAMPLES=1 -DROCKSDB_BUILD_SHARED=0 -DCMAKE_BUILD_TYPE=Debug

make -j32
make install

mkdir ${ROCKS_LIB_DIR}/bin 
cp ../build/db_bench ${ROCKS_LIB_DIR}/bin
