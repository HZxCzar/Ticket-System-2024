#ifndef ORDER_HPP
#define ORDER_HPP
#include "Database.hpp"
#include "Struct.hpp"
#include <cassert>
const string orfilename = "OrderFile";
class OrderDate {
private:
  BPT<Username,OrderVal,29> database;

public:
  OrderDate() : database(orfilename) {}
  bool buy_ticket(Username &u,TrainID &i,Time &leave,Time &arrive, Station &f, Station &t,int &cost,int &num, Status stat,int &seq);
  bool query_order(Username &u);
  bool refund_ticket(Username &u, int &n,OrderVal &odval);
  bool pending_success(Username &u,int &seq);
  void clean();
};

#endif // ORDER_HPP