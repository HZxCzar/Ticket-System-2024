#ifndef BOX_HPP
#define BOX_HPP
#include "Struct.hpp"
#include <fstream>
#include <iostream>
#include <string>
const std::string BoxName = "Box";
constexpr int Box_Size = sizeof(Box);
void Box_clear();
void Box_Write(Box &_box);
void Box_Update(Box &_box, int &index);
void Box_Read(Box &_box, int &index);
#endif // BOX_HPP