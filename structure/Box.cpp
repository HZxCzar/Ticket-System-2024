#include "Struct.hpp"
#include <fstream>
#include <iostream>
#include <string>
const std::string BoxName = "Box";
constexpr int Box_Size = sizeof(Box);
std::fstream BoxFile;
void Box_clear() {
  BoxFile.open(BoxName, std::ios::out);
  BoxFile.close();
}
void Box_Write(Box &_box) {
  BoxFile.close();
  BoxFile.open(BoxName, std::ios::out | std::ios::in | std::ios::ate);
  int pos = BoxFile.tellp();
  // std::<<"pos is "<<pos<<'\n';
  if (pos == -1) {
    BoxFile.close();
    BoxFile.open(BoxName, std::ios::out);
    BoxFile.close();
    BoxFile.open(BoxName, std::ios::out | std::ios::in | std::ios::ate);
    pos=BoxFile.tellp();
  }
  // std::<<"pos is "<<pos<<'\n';
  _box.pos = pos;
  BoxFile.write(reinterpret_cast<char *>(&_box), Box_Size);
  BoxFile.close();
}

void Box_Update(Box &_box, int &index) {
  BoxFile.close();
  BoxFile.open(BoxName, std::ios::in | std::ios::out);
  BoxFile.seekp(index);
  BoxFile.write(reinterpret_cast<char *>(&_box), Box_Size);
  BoxFile.close();
}

void Box_Read(Box &_box, int &index) {
  if (!BoxFile.is_open()) {
    BoxFile.open(BoxName, std::ios::in | std::ios::out);
  }
  BoxFile.seekg(index);
  BoxFile.read(reinterpret_cast<char *>(&_box), Box_Size);
}