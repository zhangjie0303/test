#include <iostream>

enum class A {
	a = 1,
	b = 2,
	c = 3
};

enum class B {
	d = 4,
	e = 5,
	f = 6
};

template <typename T>
auto PrintNum(T const value)-> typename std::underlying_type<T>::type {
    return static_cast<typename std::underlying_type<T>::type>(value);
}

int main() {
  auto a = A::a;
  auto b = B::e;
  std::cout << PrintNum(a) << " | " << static_cast<int>(a) << std::endl;
  //std::cout << a << std::endl;
  std::cout << PrintNum(b)<< " | " << static_cast<int>(b) << std::endl;
  //std::cout << b << std::endl;
  return 0;
}
