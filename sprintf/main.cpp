#include <stdio.h>
#include <string.h>

#include <iostream>

const int KV_MAX_LEN = 64;

int main(int argc, char** argv) {
    char buf[KV_MAX_LEN];
    memset(buf, 0, KV_MAX_LEN);
    int ret = sprintf(buf, "foo_%d", 1000);
    std::cout << "ret: " << ret << ", buf len: " << strlen(buf)
	      << ", total len: " << sizeof(buf) << ", buf: " << buf
	      << std::endl;
    return 0;
}
