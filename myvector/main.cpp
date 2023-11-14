#include <iostream>
#include <numeric>
#include <vector>
//#include <stdint.h>
#include <algorithm>
#include <random>

// A seeded replacement for removed std::random_shuffle
template <class RandomIt>
void RandomShuffle(RandomIt first, RandomIt last, uint32_t seed) {
  std::mt19937 rng(seed);
  std::shuffle(first, last, rng);
}

// A replacement for removed std::random_shuffle
template <class RandomIt>
void RandomShuffle(RandomIt first, RandomIt last) {
  RandomShuffle(first, last, std::random_device{}());
}

int main(int argc, char** argv) {
	std::vector<uint16_t> set_vec(5);
	std::iota(set_vec.begin(), set_vec.end(), static_cast<uint16_t>(0));
	std::cout << "size: " << set_vec.size() << std::endl;
	RandomShuffle(set_vec.begin(), set_vec.end());

	for(uint16_t set_i : set_vec) {
		std::cout << "set_vec[" << set_i << "]: " << set_vec[set_i] << std::endl;
	}
	return 0;
}
