#include "Train.hpp"
// #include "Box.cpp"
// #include "Comparison.hpp"
// #include "Database.hpp"
// #include "Stream/output.cpp"
#include "RemSeat.cpp"
// #include "Struct.hpp"
#include "Struct.hpp"
#include "Tools/function.hpp"
#include <cassert>
#include <iostream>
#include <string>

bool TrainDate::add_train(TrainID &i, StationNum &n, SeatNum &m, Station *s,
                          Prices *p, StartTime &x, TravelTimes *t,
                          StopoverTimes *o, SaleDate &d, Type &y) {
  Train trainset[1];
  int size = 0;
  database.Find(i, trainset, size);
  if (size > 0) {
    return false;
  }
  Box box;
  box.trainID = i;
  box.stationNum = n;
  box.seatnum = m;
  box.startTime = x;
  // std::cerr << x._startTime << "?\n";
  box.stopoverTimes[0]._stopoverTimes = 0;
  Seat_add_train(box, n, m, d);
  for (int pos = 0; pos < n._stationnum; ++pos) {
    box.station[pos] = s[pos];
    if (pos < n._stationnum - 2) {
      box.stopoverTimes[pos + 1] = o[pos]; //注意位置
    }
    if (pos < n._stationnum - 1) {
      box.price[pos] = p[pos];
      box.travelTimes[pos] = t[pos];
    }
  }
  box.stopoverTimes[n._stationnum - 1]._stopoverTimes = 0;
  box.travelTimes[n._stationnum - 1]._travelTimes = 0;
  box.price[n._stationnum - 1]._price = 0;
  Train train;
  train.released = false;
  train.saleDate = d;
  train.type = y;
  Box_Write(box);
  train.box_pos = box.pos;
  database.Insert(i, train);
  return true;
}

bool TrainDate::release_train(TrainID &i, Train &output) {
  Train trainset[1];
  int size = 0;
  database.Find(i, trainset, size);
  if (size == 0) {
    return false;
  } else if (trainset[0].released == true) {
    return false;
  } else {
    trainset[0].released = true;
    database.Modify(i, trainset[0]);
    output = trainset[0];
    // std::cerr<<"re box pos"<<trainset[0].box_pos<<'\n';
    return true;
  }
}

bool TrainDate::query_train(TrainID &i, Date &d, Train &output) {
  Train trainset[1];
  int size = 0;
  database.Find(i, trainset, size);
  if (size == 0) {
    return false;
  }
  Turn turn = MakeTurn(d);
  if (!(trainset[0].saleDate.beg._turn <= turn._turn &&
        trainset[0].saleDate.end._turn >= turn._turn)) {
    return false;
  }
  output = trainset[0];
  Box box;
  Seat seat;
  Box_Read(box, output.box_pos);
  std::cout << box.trainID << " " << output.type._Type << '\n';
  int dur = box.startTime._startTime;
  Seat_query_train(box, seat, turn);
  Date day = d;
  int price = 0;
  std::cout << box.station[0] << " xx-xx xx:xx -> " << BackDay(day) << " "
            << BackTime(dur) << " 0"
            << " " << seat.seat[0] << '\n';
  for (int i = 1; i < box.stationNum._stationnum - 1; ++i) {
    price += box.price[i - 1]._price;
    dur += box.travelTimes[i - 1]._travelTimes;
    day._date += dur / (24 * 60);
    MainTainDate(day._date);
    dur = dur % (24 * 60);
    std::cout << box.station[i] << " " << BackDay(day) << " " << BackTime(dur)
              << " -> ";
    // std::cout << box.station[i] << " " << turn._turn + dur / (24 * 60) << " "
    // << dur % (24 * 60) << " -> ";
    dur += box.stopoverTimes[i]._stopoverTimes;
    day._date += dur / (24 * 60);
    MainTainDate(day._date);
    dur = dur % (24 * 60);
    std::cout << BackDay(day) << " " << BackTime(dur) << " " << price << " "
              << seat.seat[i] << '\n';
  }
  dur += box.travelTimes[box.stationNum._stationnum - 2]._travelTimes;
  day._date += dur / (24 * 60);
  dur = dur % (24 * 60);
  MainTainDate(day._date);
  std::cout << box.station[box.stationNum._stationnum - 1] << " "
            << BackDay(day) << " " << BackTime(dur) << " -> xx-xx xx:xx "
            << price + box.price[box.stationNum._stationnum - 2]._price << " x"
            << '\n';
  return true;
}

bool TrainDate::delete_train(TrainID &i, Train &output) {
  Train trainset[1];
  int size = 0;
  database.Find(i, trainset, size);
  if (size == 0) {
    return false;
  }
  if (trainset[0].released) {
    return false;
  }
  output = trainset[0];
  database.Delete(i, trainset[0]);
  return true;
}

// bool TrainDate::query_ticket(TrainID *set, int &num, Station &s, Station &t,
//                              Date &d, bool &sort_type) {
//   Train trainmsg[2];
//   int size;
//   for (int pos = 0; pos < num; ++pos) {
//     size = 0;
//     database.Find(set[pos], trainmsg, size);
//     assert(size == 1);
//     if (!legal_date(trainmsg[0].saleDate, d)) {
//       continue;
//     }
//     if(!check_station_sequence(trainmsg[0],s,t))
//     {
//       continue;
//     }
//   }
// }

int TrainDate::buy_ticket(Username &u, TrainID &i, Date &d, int &n, Station &f,
                          Station &t, bool &bp, int &seq) {
  Train set[1];
  int size = 0;
  database.Find(i, set, size);
  assert(size <= 1);
  if (size == 0) {
    return -1;
  }
  if (set[0].released == false) {
    std::cout << "-1\n";
    return -1;
  }
  Train train = set[0];
  Box box;
  Box_Read(box, train.box_pos);
  if (!legal_date(train.saleDate, box, f, d)) {
    std::cout << "-1\n";
    return -1;
  }
  // std::cerr<<box.trainID<<'\n';
  if (!check_station_sequence(box, f, t)) {
    std::cout << "-1\n";
    return -1;
  }
  int minseat = 114514;
  Turn turn = seek_turn_from_station(box, f, d);
  Seat seat;
  Seat_query_train(box, seat, turn);
  bool flag = false;
  int cost = 0;
  int day = turn._turn + 600; //日期
  int beg = box.startTime._startTime, dur = 0;
  for (int i = 0; i < box.stationNum._stationnum; ++i) {
    if (box.station[i] == f) {
      flag = true;
      beg += box.stopoverTimes[i]._stopoverTimes;
      dur = box.travelTimes[i]._travelTimes;
      cost += n * box.price[i]._price;
      if (seat.seat[i] < minseat) {
        minseat = seat.seat[i];
      }
      continue;
    }
    if (box.station[i] == t) {
      break;
    }
    if (flag) {
      dur +=
          box.stopoverTimes[i]._stopoverTimes + box.travelTimes[i]._travelTimes;
      cost += n * box.price[i]._price;
      if (seat.seat[i] < minseat) {
        minseat = seat.seat[i];
      }
    } else {
      beg +=
          box.stopoverTimes[i]._stopoverTimes + box.travelTimes[i]._travelTimes;
    }
  }
  Time leave, arrive;
  leave._date = day + beg / (24 * 60);
  leave._time = beg % (24 * 60);
  if (leave._date / 100 == 6 && leave._date % 100 > 30) {
    int tmp = leave._date - 630;
    leave._date = 700 + tmp;
  }
  if (leave._date / 100 == 7 && leave._date % 100 > 31) {
    int tmp = leave._date - 731;
    leave._date = 800 + tmp;
  }
  if (leave._date / 100 >= 8 && leave._date % 100 > 31) {
    int tmp = leave._date - 831;
    leave._date = 900 + tmp;
  }
  arrive._date = day + (beg + dur) / (24 * 60);
  arrive._time = (beg + dur) % (24 * 60);
  // std::cerr<<arrive._date<<"date<<'\n";
  if (arrive._date / 100 == 6 && arrive._date % 100 > 30) {
    int tmp = arrive._date - 630;
    arrive._date = 700 + tmp;
  }
  if (arrive._date / 100 == 7 && arrive._date % 100 > 31) {
    int tmp = arrive._date - 731;
    arrive._date = 800 + tmp;
  }
  if (arrive._date / 100 >= 8 && arrive._date % 100 > 31) {
    int tmp = arrive._date - 831;
    arrive._date = 900 + tmp;
  }
  int price = cost / n;
  // std::cerr<<"left:"<<minseat<<'\n';
  if (minseat >= n) {
    std::cout << cost << '\n';
    flag = false;
    for (int i = 0; i < box.stationNum._stationnum; ++i) {
      if (box.station[i] == f) {
        flag = true;
        seat.seat[i] -= n;
        assert(seat.seat[i] >= 0);
        continue;
      }
      if (box.station[i] == t) {
        break;
      }
      if (flag) {
        seat.seat[i] -= n;
        assert(seat.seat[i] >= 0);
      }
    }
    Seat_update_train(box, seat, turn);
    // std::cerr << "leavedate" << leave._date << '\n';
    order.buy_ticket(u, i, leave, arrive, f, t, price, n, success, seq);
    return 0;
  } else {
    if (bp) {
      if (n > box.seatnum._seatnum) {
        std::cout << "-1\n";
        return -1;
      }
      queue.add_queue(u, i, turn, f, t, n, seq);
      order.buy_ticket(u, i, leave, arrive, f, t, price, n, pending, seq);
      std::cout << "queue\n";
      return 1;
    } else {
      std::cout << "-1\n";
      return -1;
    }
  }
}

bool TrainDate::query_order(Username &u) { return order.query_order(u); }

bool TrainDate::refund_ticket(Username &u, int &n, string &seq) {
  OrderVal odval;
  if (order.refund_ticket(u, n, odval)) {
    Train set[1];
    int size = 0;
    database.Find(odval.trainID, set, size);
    assert(size == 1);
    Box box;
    Box_Read(box, set[0].box_pos);
    Seat seat;
    Date leave;
    leave._date = odval.leaving_time._date;
    // std::cerr << "leave:" << odval.leaving_time._date << '\n';
    Turn turn = seek_turn_from_station(box, odval.from, leave);
    if (odval.status == pending) {
      // if (odval.seq == 99294) {
      //   std::cout << "what can I say!!!\n";
      //   assert(0);
      // }
      queue.erase_queue(odval.trainID, turn, odval.seq);
      return true;
    }
    Seat_query_train(box, seat, turn);
    bool flag = false;
    for (int i = 0; box.stationNum._stationnum; ++i) {
      if (box.station[i] == odval.from) {
        flag = true;
      }
      if (box.station[i] == odval.to) {
        break;
      }
      if (flag) {
        seat.seat[i] += odval.ticketNum;
      }
    }
    Seat_update_train(box, seat, turn);
    QueVal update[1001];
    size = 0;
    if (queue.try_update(box, turn, seat, update, size)) {
      Seat_update_train(box, seat, turn);
      for (int pos = 0; pos < size; ++pos) {
        // if (update[pos].seq == 99294) {
        //   std::cout << "what can I say!!\n";
        //   assert(0);
        // }
        // std::cerr<<update[pos].username<<" "<<update[pos].seq<<'\n';
        queue.erase_queue(odval.trainID, turn, update[pos].seq);
        // if (seq == "[343567]" &&pos==1) {
        //   std::cout <<size<<'\n';
        //   return false;
        // }
        order.pending_success(update[pos].username, update[pos].seq);
      }
    }
    return true;
  } else {
    return false;
  }
}

void TrainDate::clean() {
  database.clear();
  order.clean();
  queue.clean();
  Seat_clean();
}