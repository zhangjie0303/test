#include <iostream>

int main(int argc, char** argv) {
	//const char* cstr = "hello\0\0";
	//std::cout << "cstr: " << cstr << std::endl;

	//std::string str(cstr, 6);
	std::string  str("hello\0\0abc", 10);
	std::string str1 = "hello";
	std::cout << "str: " << str << ", size: " << str.size() << std::endl;
	std::cout << "str1: " << str1 << ", size: " << str1.size() << std::endl;

	return 0;
}
