//#include <time.h>
#include <unistd.h>

#include <chrono>
#include <iostream>

int main(int argc, char** argv) {
    auto start = std::chrono::system_clock::now();
    ::usleep(8000);
    auto end = std::chrono::system_clock::now();
    // std::chrono::duration<double> diff = end - start;
    auto duration =
	std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "duration: " << duration.count() << "ms." << std::endl;
}
