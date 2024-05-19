#ifndef REMSEAT_HPP
#define REMSEAT_HPP
// #include "RemSeat.hpp"
// #include "Box.cpp"
#include "Comparison.hpp"
#include "Struct.hpp"
// #include "Database.hpp"
// #include "Stream/output.cpp"
// #include "Struct.hpp"
// #include "Tools/function.hpp"
#include <fstream>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

const std::string seatfilename = "RemSeatFile";
std::fstream seatFile;
constexpr int SeatBlockSize = sizeof(Seat);


bool Seat_add_train(Box &box, StationNum &n, SeatNum &m, SaleDate &d) {
  seatFile.close();
  seatFile.open(seatfilename, std::ios::out | std::ios::in | std::ios::ate);
  Seat seat;
  // std::memset(seat.seat, m._seatnum, sizeof(seat));
  int pos=seatFile.tellp();
  if (pos == -1) {
    seatFile.close();
    seatFile.open(seatfilename, std::ios::out);
    seatFile.close();
    seatFile.open(seatfilename, std::ios::out | std::ios::in | std::ios::ate);
  }
  for(int i=0;i<=d.end._turn-d.beg._turn;++i)
  {
    seat.seat[i]=m._seatnum;
  }
  // std::<<m._seatnum<<"<<<\n";
  for (int i = d.beg._turn; i <= d.end._turn; ++i) {
    pos = seatFile.tellp();
    box.SeatBlockPos[i] = pos;
    seatFile.write(reinterpret_cast<char *>(&seat), SeatBlockSize);
  }
  seatFile.close();
  return true;
}

bool Seat_query_train(Box &box,Seat &seat, Turn turn) {
  if (!seatFile.is_open()) {
    seatFile.open(seatfilename, std::ios::out | std::ios::in | std::ios::ate);
  }
  seatFile.seekg(box.SeatBlockPos[turn._turn]);
  seatFile.read(reinterpret_cast<char *>(&seat), SeatBlockSize);
  return true;
}

bool Seat_update_train(Box &box,Seat &seat,Turn turn)
{
  seatFile.close();
  seatFile.open(seatfilename, std::ios::out | std::ios::in | std::ios::ate);
  seatFile.seekp(box.SeatBlockPos[turn._turn]);
  seatFile.write(reinterpret_cast<char *>(&seat), SeatBlockSize);
  seatFile.close();
  return true;
}

void Seat_clean()
{
  seatFile.open(seatfilename, std::ios::out);
  seatFile.close();
}

// bool RemSeatDate::delete_train(int &box_pos, SaleDate &s) {
//   SeatKey seatkey;
//   SeatVal seatval;
//   seatkey.box_pos = box_pos;
//   for (int pos = s.beg._turn; pos <= s.end._turn; ++pos) {
//     seatkey.turn._turn = pos;
//     seatval.turn._turn = pos;
//     database.Delete(seatkey, seatval);
//   }
//   return true; // to modify
// }
#endif