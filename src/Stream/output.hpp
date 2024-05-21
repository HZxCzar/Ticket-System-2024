#ifndef OUTPUT_HPP
#define OUTPUT_HPP
#include <iostream>
#include <ostream>
#include <string>
#include "../Struct.hpp"
using std::ostream;
inline ostream &operator<<(ostream &os, const UserInfo &s);
inline ostream &operator<<(ostream &os,const Username &s);
inline ostream &operator<<(ostream &os,const Privilege &s);
inline ostream &operator<<(ostream &os,Time s);
inline ostream &operator<<(ostream &os,const Station &s);
inline ostream &operator<<(ostream &os,const TrainID &s);
#endif