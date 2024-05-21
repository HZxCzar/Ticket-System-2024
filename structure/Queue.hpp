#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "Database.hpp"
#include <cassert>
const string quefilename = "QueueFile";
class QueueDate {
private:
  BPT<QueKey,QueVal,34> database;

public:
  QueueDate() : database(quefilename) {}
  bool add_queue(Username &u,TrainID &i,Turn &turn,Station &beg,Station &end,int &num,int &seq);
  bool erase_queue(TrainID &i,Turn &turn,int &seq);
  bool try_update(Box &box,Turn &turn,Seat &seat,QueVal *set,int &size);
  void clean();
};
#endif // QUEUE_HPP