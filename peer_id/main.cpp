#include <iostream>
#include <string>

int main(int argc, char** argv) {
  std::string str = "1.1.1.1:8900:1";
  char ip_str[64];
  int port = 0;
  int idx = 0;
  int ret = sscanf(str.c_str(), "%[^:]%*[:]%d%*[:]%d", ip_str, port, &idx);
  //sscanf(str.c_str(), "%[^:]%*[:]%d%*[:]%d", ip_str, &addr.port, &idx)
  std::cout << "ret = " << ret << std::endl;
  //std::cout << "ret = " << ret << ", ip_str: " << ip_str << ", port: " << port << ", idx: " << idx << std::endl;
}
