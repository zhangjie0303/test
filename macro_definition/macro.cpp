#include <iostream>

struct Student {
  int id;
  std::string name;
};

#define PRINT(stu, field) { \
  std::cout << stu.field << std::endl; \
}

int main(int argc, char** argcv) {
  Student stu;
  stu.id = 888;
  stu.name = "amdin";
  // std::cout << stu.name<< std::endl;
  PRINT(stu, name);
  return 0;
}
