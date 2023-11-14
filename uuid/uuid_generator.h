#include <sstream>
#include <random>

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
