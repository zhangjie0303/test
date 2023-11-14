#include <iostream>

inline static const uint32_t kAutoIncrementOffsetMax = 65535;

uint64_t GetRealStartId(uint64_t start_id, uint32_t auto_increment_increment,
                                              uint32_t auto_increment_offset) {
  uint64_t remainder = start_id % auto_increment_increment;
  if (remainder < auto_increment_offset) {
    return start_id - remainder + auto_increment_offset;
  } else if (remainder > auto_increment_offset) {
    return start_id - remainder + auto_increment_increment + auto_increment_offset;
  }

  //if (auto_increment_offset == auto_increment_increment) {
  //if (auto_increment_offset == remainder) {
    return start_id;
  //}
  //return start_id + auto_increment_offset;
}

uint64_t GetGenerateEndId(uint64_t start_id, uint32_t count, uint32_t increment,
                                                uint32_t offset) {
  if (increment == 0 || increment > kAutoIncrementOffsetMax) {
    std::cout << "WARNING " << "invalid auto_increment_increment: " << increment << ", set to default value 1.";
    increment = 1;
  }

  if (offset == 0 || offset > kAutoIncrementOffsetMax || offset > increment) {
    std::cout << "WARNING " << "invalid auto_increment_offset: " << offset << ", set to default value 1.";
    offset = 1;
  }

  if (increment == 1 && offset == 1) {
    return start_id + count;
  }

  uint64_t real_start_id = GetRealStartId(start_id, increment, offset);
  std::cout << "real_start_id: " << real_start_id << std::endl;
  //return real_start_id + (count - 1) * increment + 1;
  return real_start_id + count * increment;
}

int main(int argc, char** argv) {
  if(argc < 5) {
    std::cout << "params must equal 4, useage: " << argv[0] << " <start_id> <count> <increment> <offset>" << std::endl;
    return -1;
  }
  uint64_t start_id = uint64_t(std::stoi(argv[1]));
  uint32_t count = uint32_t(std::stoi(argv[2]));
  uint32_t increment = uint32_t(std::stoi(argv[3]));
  uint32_t offset = uint32_t(std::stoi(argv[4]));

  std::cout << "input --> " << " start_id: " << start_id << ", count: " << count << ", increment: " << increment << ", offset: " << offset << std::endl;

  uint64_t end_id = GetGenerateEndId(start_id, count, increment, offset);
  std::cout << "output --> " << " [" << start_id << ", " << end_id << ")" << std::endl;

  // [start_id, end_id):
  // count
  // end_id = real_start_id + (count - 1) * auto_increment_increment + 1;
  return 0;
}

