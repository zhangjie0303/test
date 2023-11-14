#include <sys/time.h>
#include <unistd.h>

#include <iostream>

uint64_t get_now_ms() {
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 + now.tv_usec / 1000;
}

uint64_t get_now_s() {
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec;
}

int main(int argc, char *argv[]) {
    uint64_t start = get_now_ms();
    ::usleep(5000);
    uint64_t end = get_now_ms();
    std::cout << "cost: " << end - start << std::endl;

    return 0;
}

