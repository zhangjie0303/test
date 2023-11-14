#if 0
#include <bit>
#include <iostream>

int main()
{
    float f = std::numeric_limits<float>::infinity();
    int i = std::bit_cast<int>(f);
    std::cout << "float f = " << std::hex << f
              << "\nstd::bit_cast<int>(f) = " << std::hex << i << '\n';
    return 0;
}
#else

#include <cstdint>
#include <bit>
#include <iostream>
 
constexpr double f64v = 19880124.0; 
constexpr auto u64v = std::bit_cast<std::uint64_t>(f64v);
static_assert( std::bit_cast<double>(u64v) == f64v ); // round-trip
 
constexpr std::uint64_t u64v2 = 0x3fe9000000000000ull;
constexpr auto f64v2 = std::bit_cast<double>(u64v2);
static_assert( std::bit_cast<std::uint64_t>(f64v2) == u64v2 ); // round-trip
 
int main()
{
    std::cout
        << "std::bit_cast<std::uint64_t>(" << std::fixed << f64v << ") == 0x"
        << std::hex << u64v << '\n'
        << "std::bit_cast<double>(0x" << std::hex << u64v2 << ") == "
        << std::fixed << f64v2 << '\n';
}

#endif
