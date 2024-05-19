#ifndef STATION_HPP
#define STATION_HPP

#include "Comparison.hpp"
#include "Database.hpp"
#include "Struct.hpp"
#include <cassert>
const string filename = "StationFile";
class StationDate {
private:
  BPT<Station,Train,87> database;

public:
  StationDate() : database(filename) {}
  bool release_train(Train &t);
  // bool delete_train();
  bool query_ticket(Station &s,Station &t,Date &d,bool &sort_type,string &token0);
  bool query_transfer(Station &s,Station &t,Date &d,bool &sort_type);
  void clean();
};

#endif // STATION_HPP