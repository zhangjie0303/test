#include <iostream>

#if 0
template<class T>
struct trait;

class class_ {
      class impl;
};

// Not allowed in standard C++ (impl is private)
template<>
struct trait<class_::impl>;
#endif

constexpr auto add1(auto lhs, auto rhs) {
    return lhs + rhs;
}
using T = decltype(add1(std::declval<int>(), std::declval<int>()));

int main(int argc, char** argv) {
    std::cout << "zhangjie!!!" << std::endl;
    std::cout << add1(1, 2) << std::endl;
    //std::cout << T(3, 2) << std::endl;
}

// clang++ -std=c++20 -o featere feature.cpp  // OK
// clang++ -std=c++20 -o featere feature.cpp  // NOT OK
