#ifndef TRAIN_HPP
#define TRAIN_HPP

// #include "Comparison.hpp"
#include "Database.hpp"
#include "Order.hpp"
#include "Order.cpp"
#include "Queue.hpp"
#include "Queue.cpp"
#include "Struct.hpp"
#include <cassert>
const string tfilename = "TrainFile";
class TrainDate {
private:
  BPT<TrainID, Train,110> database;
  OrderDate order;
  QueueDate queue;

public:
  TrainDate() : database(tfilename) {}
  bool add_train(TrainID &i,StationNum &n,SeatNum &m,Station *s,Prices *p,StartTime &x,TravelTimes *t,StopoverTimes *o,SaleDate &d,Type &y);
  bool release_train(TrainID &i,Train &output);
  bool query_train(TrainID &i,Date &d,Train &output);
  bool delete_train(TrainID &i,Train &output);
  // bool query_ticket(TrainID *set,int &num,Station &s,Station &t,Date &d,bool &sort_type);
  // bool query_transfer();
  int buy_ticket(Username &u,TrainID &i,Date &d,int &n,Station &f,Station &t,bool &bp,int &seq);
  bool query_order(Username &u);
  bool refund_ticket(Username &u,int &n,string &seq);
  void clean();
};

#endif // TRAIN_HPP