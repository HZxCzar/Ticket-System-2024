#ifndef REMSEAT_HPP
#define REMSEAT_HPP
#include "Comparison.hpp"
#include "Struct.hpp"
#include <fstream>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
const std::string seatfilename = "RemSeatFile";
constexpr int SeatBlockSize = sizeof(Seat);

bool Seat_add_train(Box &box, StationNum &n, SeatNum &m, SaleDate &d);
bool Seat_query_train(Box &box,Seat &seat, Turn turn);
bool Seat_update_train(Box &box,Seat &seat,Turn turn);
void Seat_clean();

#endif // REMSEAT_HPP