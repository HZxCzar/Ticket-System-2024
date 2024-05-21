#include "Queue.hpp"

bool QueueDate::add_queue(Username &u, TrainID &i, Turn &turn, Station &beg,
                          Station &end, int &num, int &seq) {
  QueKey key;
  QueVal val;
  key.trainID = i;
  key.turn = turn;
  val.seq = seq;
  val.ticketNum = num;
  val.begStation = beg;
  val.endStation = end;
  val.username = u;
  database.Insert(key, val);
  return true;
}

bool QueueDate::erase_queue(TrainID &i, Turn &turn, int &seq) {
  QueKey key;
  QueVal val;
  key.trainID = i;
  key.turn = turn;
  val.seq = seq;
  database.Delete(key, val);
  return true;
}

bool QueueDate::try_update(Box &box, Turn &turn, Seat &seat, QueVal *set,
                           int &size) {
  QueKey key;
  QueVal val[1001];
  key.trainID = box.trainID;
  key.turn = turn;
  int num = 0;
  size = 0;
  database.Find(key, val, num);
  if (num == 0) {
    return false;
  }
  if(num>1001){
    throw "try_update invalid bound";
  }
  for (int i = 0; i < num; ++i) {
    int mini = 114514, pos1, pos2;
    bool beg = false;
    for (int pos = 0; pos < box.stationNum._stationnum; ++pos) {

      if (box.station[pos] == val[i].begStation) {
        beg = true;
        pos1 = pos;
      }
      if (box.station[pos] == val[i].endStation) {
        pos2 = pos;
        break;
      }
      if (beg) {
        if (seat.seat[pos] < mini) {
          mini = seat.seat[pos];
        }
      }
    }
    if (mini >= val[i].ticketNum) {
      set[size++] = val[i];
      for (int change = pos1; change < pos2; ++change) {
        seat.seat[change] -= val[i].ticketNum;
      }
    }
  }
  if (size == 0) {
    return false;
  }
  return true;
}

void QueueDate::clean() { database.clear(); }