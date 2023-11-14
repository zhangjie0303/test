#include <iostream>
//#include <sstream>
//#include <iomanip>
//#include <chrono>
//#include <functional>
//#include <random>
#include "time_helper.h"
#include "uuid_generator.h"

/*
auto g_random_seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 g_seed_engine(g_random_seed);

std::string generate_uuid()
{
    std::stringstream stream;
    std::uniform_int_distribution<std::size_t> random_gen ;
    std::size_t value = random_gen(g_seed_engine);
    stream << std::hex << value;
    
    return stream.str();
}
*/

int main(int argc, char** argv) {
	//std::cout << Uuid::create_uuid() << std::endl;
	std::cout << generate_uuid() << std::endl;
	std::cout << generate_uuid() << std::endl;
	std::cout << TimeHelper::micros() << std::endl;
	std::cout << std::to_string(TimeHelper::micros()) << std::endl;
}
