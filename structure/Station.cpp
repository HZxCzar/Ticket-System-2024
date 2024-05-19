#include "Station.hpp"
// #include "Box.cpp"
// #include "Comparison.hpp"
#include "./Stream/output.cpp"
#include "Components/map.hpp"
#include "Components/utility.hpp"
#include "Database.hpp"
// #include "Stream/output.cpp"
// #include "Struct.hpp"
#include "RemSeat.cpp"
#include "Struct.hpp"
#include "Tools/function.hpp"
#include <bits/types/struct_tm.h>
#include <cassert>
#include <iostream>

bool StationDate::release_train(Train &t) {
  Box box;
  Box_Read(box, t.box_pos);
  for (int pos = 0; pos < box.stationNum._stationnum; ++pos) {
    database.Insert(box.station[pos], t);
  }
  return true;
}
Train trainA[5000];
Train trainB[5000];
Box box[2001];
int timi[5000];
int miniseat[5000];
int price[5000];
int leave[5000];
int indexi[5000];
bool StationDate::query_ticket(Station &s, Station &t, Date &d, bool &sort_type,
                               string &token0) {
  // int timi[5000];
  // int miniseat[501];
  // int price[501];
  // int leave[501];
  // int indexi[501];
  int num = 0;
  int sizeA = 0, sizeB = 0;
  database.Find(s, trainA, sizeA);
  database.Find(t, trainB, sizeB);
  // if(token0=="[5774722]")
  // {
  //   std::cout<<sizeA<<" "<<sizeB<<'\n';
  //   return false;
  // }
  // std::cerr<<"sizeA:"<<sizeA<<"|sizeB:"<<sizeB<<" "<<t<<'\n';
  // std::cerr<<trainA[0].box_pos<<" "<<trainA[1].box_pos<<"
  // "<<trainB[0].box_pos<<'\n';
  //以box_pos为顺序
  int posA = 0, posB = 0;
  while (posA < sizeA && posB < sizeB) {
    if (trainA[posA].box_pos < trainB[posB].box_pos) {
      ++posA;
    } else if (trainA[posA].box_pos > trainB[posB].box_pos) {
      ++posB;
    } else {
      // std::cerr<<box[num].trainID<<"|"<<trainA[posA].box_pos<<"-"<<trainB[posB].box_pos<<'\n';
      Box_Read(box[num], trainA[posA].box_pos);
      if (!legal_date(trainA[posA].saleDate, box[num], s, d)) {
        // if(token0=="[491552]" && box[num].trainID._trainID[0]=='i')
        // {
        //   std::cout<<"date error"<<box[num].trainID<<'\n';
        // }
        ++posA, ++posB;
        continue;
      }
      // std::cerr<<box[num].trainID<<"|"<<trainA[posA].box_pos<<"-"<<trainB[posB].box_pos<<'\n';
      if (!check_station_sequence(box[num], s, t)) {
        ++posA, ++posB;
        continue;
      }
      bool beg = false;
      leave[num] = box[num].startTime._startTime;
      // std::cerr<<leave[num]/60<<" "<<leave[num]%60<<"<<<\n";
      Seat seat;
      // std::cerr << BackDay(d) << '\n';
      Turn begin = seek_turn_from_station(box[num], s, d);
      Seat_query_train(box[num], seat, begin);
      miniseat[num] = 1145140;
      // std::cerr<<price[num]<<"<<<\n";
      price[num] = 0;
      timi[num] = 0;
      price[num] = 0;
      for (int pos = 0; pos < box[num].stationNum._stationnum; ++pos) {
        if (box[num].station[pos] == s) {
          beg = true;
          if (seat.seat[pos] < miniseat[num]) {
            // std::cerr<<seat.seat[pos]<<"[seat]
            // :"<<box[num].station[pos]<<'\n';
            miniseat[num] = seat.seat[pos];
          }
          leave[num] += box[num].stopoverTimes[pos]._stopoverTimes;
          timi[num] += box[num].travelTimes[pos]._travelTimes;
          price[num] += box[num].price[pos]._price;
          continue;
        }
        if (!beg) {
          leave[num] += box[num].travelTimes[pos]._travelTimes +
                        box[num].stopoverTimes[pos]._stopoverTimes;
        }
        if (box[num].station[pos] == t) {
          // if (seat.seat[pos] < miniseat[num])
          // {
          //   miniseat[num] = seat.seat[pos];
          // }
          break;
        }
        if (beg) {
          if (seat.seat[pos] < miniseat[num]) {
            // std::cerr << seat.seat[pos] << "[seat]" << ' ';
            miniseat[num] = seat.seat[pos];
          }

          price[num] += box[num].price[pos]._price;
          timi[num] += box[num].stopoverTimes[pos]._stopoverTimes +
                       box[num].travelTimes[pos]._travelTimes;
        }
      }
      indexi[num] = num;
      // std::cerr<<leave[num]/60<<" "<<leave[num]%60<<"<<\n";
      ++num;
      ++posA;
      ++posB;
    }
  }
  if (num == 0) {
    std::cout << "0\n";
    return false;
  }
  // if(token0=="[4607628]"){
  //   std::cout<<"wa\n";
  //   return 0;
  // }
  if (sort_type) {
    // std::cerr<<"???\n";
    // std::cerr<<timi[0]<<" "<<timi[1]<<'\n';
    MySort(timi, indexi, box, 0, num - 1);
    // std::cerr<<indexi[0]<<" "<<indexi[1]<<'\n';
  } else {
    MySort(price, indexi, box, 0, num - 1);
  }
  // if(token0=="[4607628]")
  // {
  //   std::cout<<num<<'\n';
  //   return 0;
  // }
  std::cout << num << '\n';
  string backday1,backday2,backtime1,backtime2;
  Date ar;
  for (int i = 0; i < num; ++i) {
    // std::cerr<<"turn is"<<MakeTurn(d)._turn<<'\n';
    ar._date =
        d._date + (leave[indexi[i]] % (24 * 60) + timi[indexi[i]]) / (24 * 60);
    if (ar._date / 100 == 6 && ar._date % 100 > 30) {
      int tmp = ar._date - 630;
      ar._date = 700 + tmp;
    }
    if (ar._date / 100 == 7 && ar._date % 100 > 31) {
      int tmp = ar._date - 731;
      ar._date = 800 + tmp;
    }
    if (ar._date / 100 == 8 && ar._date % 100 > 31) {
      int tmp = ar._date - 831;
      ar._date = 900 + tmp;
    }
    
    backday1=BackDay(d);
    backtime1=BackTime(leave[indexi[i]] % (24 * 60));
    backday2=BackDay(ar);
    backtime2=BackTime((leave[indexi[i]] + timi[indexi[i]]) % (24 * 60));
    std::cout << box[indexi[i]].trainID << ' ' << s << ' ' << backday1 << " "
              << backtime1 << " -> " << t << ' '
              << backday2 << " "
              << backtime2 << ' '
              << price[indexi[i]] << ' ' << miniseat[indexi[i]] << '\n';
  }
  return true;
}
Box boxA[5000];
Box boxB[5000];
bool StationDate::query_transfer(Station &s, Station &t, Date &d,
                                 bool &sort_type) {
  // Train trainA[101];
  // Train trainB[101];
  // // Train set[101];
  // Box boxA[101];
  // Box boxB[101];
  // int timi[101];
  // int price[101];
  int num = 0;
  int sizeA = 0, sizeB = 0;
  database.Find(s, trainA, sizeA);
  database.Find(t, trainB, sizeB);
  if (sizeA == 0 || sizeB == 0) {
    std::cout << "0\n";
    return false;
  }
  //以box_pos为顺序
  int posA = 0, posB = 0;
  for (int i = 0; i < sizeA; ++i) {
    Box_Read(boxA[posA], trainA[i].box_pos);
    if (!legal_date(trainA[i].saleDate, boxA[posA], s, d)) {
      continue;
    }
    posA++;
  }
  for (int i = 0; i < sizeB; ++i) {
    Box_Read(boxB[posB], trainB[i].box_pos);
    // if (!legal_date(trainB[i].saleDate,boxB[posB],t, d)) {
    //   continue;
    // }
    posB++;
  }
  // sjtu::pair<int,int> tm;
  sjtu::map<Station, sjtu::pair<int, sjtu::pair<int, int>>> myMap;
  bool flag;
  int tm, ps;
  int possibleA = -1, possibleB = -1;
  int cost = 1145140, secondcost = 1145140;
  // TrainID fir, sec;
  Station MEET;
  for (int a = 0; a < posA; ++a) {
    myMap.clear();
    flag = false;
    int begintimi, endtimi;
    tm = boxA[a].startTime._startTime;
    ps = 0;
    for (int inp = 0; inp < boxA[a].stationNum._stationnum; ++inp) {
      if (boxA[a].station[inp] == s) {
        flag = true;
        begintimi = tm + boxA[a].stopoverTimes[inp]._stopoverTimes;
        tm += boxA[a].stopoverTimes[inp]._stopoverTimes +
              boxA[a].travelTimes[inp]._travelTimes;
        ps += boxA[a].price[inp]._price;
        continue;
      }
      if (flag) {
        sjtu::pair<int, int> msg(tm, ps);
        sjtu::pair<int, sjtu::pair<int, int>> val(a, msg);
        sjtu::pair<Station, sjtu::pair<int, sjtu::pair<int, int>>> value(
            boxA[a].station[inp], val);
        myMap.insert(value);
        ps += boxA[a].price[inp]._price;
      }
      tm += boxA[a].stopoverTimes[inp]._stopoverTimes +
            boxA[a].travelTimes[inp]._travelTimes;
    }
    Turn StratTurn = seek_turn_from_station(boxA[a], s, d);
    for (int b = 0; b < sizeB; ++b) {
      flag = false;
      ps = 0;
      if (boxB[b].trainID == boxA[a].trainID) {
        // std::cout<<sizeA<<" "<<sizeB<<'\n';
        // std::cout<<boxA[0].trainID<<" "<<boxA[0].trainID<<'
        // '<<boxB[1].trainID<<'\n';
        continue;
      }
      tm = boxB[b].startTime._startTime;
      for (int x = 0; x < boxB[b].stationNum._stationnum; ++x) {
        tm += boxB[b].stopoverTimes[x]._stopoverTimes +
              boxB[b].travelTimes[x]._travelTimes;
        // ps+=boxB[b].price[x]._price;
      }
      for (int inp = boxB[b].stationNum._stationnum - 1; inp >= 0; --inp) {
        tm -= boxB[b].stopoverTimes[inp]._stopoverTimes +
              boxB[b].travelTimes[inp]._travelTimes;
        if (boxB[b].station[inp] == t) {
          flag = true;
          // tm-=boxB[b].stopoverTimes[inp]._stopoverTimes+boxB[b].travelTimes[inp]._travelTimes;
          endtimi = tm;
          continue;
        }
        if (flag) {
          ps += boxB[b].price[inp]._price;
          if (myMap.count(boxB[b].station[inp])) {
            tm += boxB[b].stopoverTimes[inp]._stopoverTimes;
            sjtu::map<Station, sjtu::pair<int, sjtu::pair<int, int>>>::iterator
                it = myMap.find(boxB[b].station[inp]);
            int timicost = 0, pricecost = 0;
            int Aturn = StratTurn._turn + it->second.second.first / (24 * 60);
            if (Aturn <= (trainB[b].saleDate.end._turn + tm / (24 * 60))) {
              int can_add =
                  trainB[b].saleDate.end._turn + tm / (24 * 60) - Aturn;
              // std::cerr<<boxB[b].trainID<<'
              // '<<trainB[b].saleDate.beg._turn<<"|"<<trainB[b].saleDate.end._turn<<"Aturn:"<<Aturn<<"tm"<<tm<<"\n";
              bool if_move = false;
              while ((trainB[b].saleDate.beg._turn + tm / (24 * 60)) > Aturn &&
                     can_add > 0) {
                timicost += 24 * 60;
                // std::cerr<<"move day"<<timicost<<'\n';
                if_move = true;
                --can_add;
                ++Aturn;
              }
              if ((trainB[b].saleDate.beg._turn + tm / (24 * 60)) <= Aturn) {
                if (tm % (24 * 60) >= (it->second.second.first % (24 * 60))) {
                  timicost +=
                      endtimi - tm +
                      (tm % (24 * 60) - (it->second.second.first % (24 * 60))) +
                      it->second.second.first - begintimi;
                  // std::cerr<<"Final add"<<timicost<<'\n';
                } else {
                  if (can_add >= 1) {
                    timicost += endtimi - tm +
                                (tm % (24 * 60) -
                                 (it->second.second.first % (24 * 60))) +
                                24 * 60 + it->second.second.first - begintimi;
                    --can_add;
                    if (if_move)
                      timicost -= 24 * 60;
                    // std::cerr<<"Final add plus"<<timicost<<'\n';
                  } else {
                    timicost = 0;
                  }
                }
              } else {
                timicost = 0;
              }
            }
            if (timicost != 0) {
              pricecost = it->second.second.second + ps;
              if (sort_type) {
                if (timicost < cost) {
                  // std::cerr<<"modify"<<timicost<<"|"<<cost<<"|"<<boxB[b].trainID<<'\n';
                  cost = timicost;
                  secondcost = pricecost;
                  possibleA = it->second.first;
                  // fir = boxA[it->second.first].trainID;
                  possibleB = b;
                  // sec = boxB[b].trainID;
                  MEET = it->first;
                } else if (timicost == cost) {
                  if (pricecost < secondcost) {
                    cost = timicost;
                    secondcost = pricecost;
                    possibleA = it->second.first;
                    // fir = boxA[it->second.first].trainID;
                    possibleB = b;
                    // sec = boxB[b].trainID;
                    MEET = it->first;
                  } else if (pricecost == secondcost) {
                    if (boxA[it->second.first].trainID <
                        boxA[possibleA].trainID) {
                      cost = timicost;
                      secondcost = pricecost;
                      possibleA = it->second.first;
                      // fir = boxA[it->second.first].trainID;
                      possibleB = b;
                      // sec = boxB[b].trainID;
                      MEET = it->first;
                    } else if (boxA[it->second.first].trainID ==
                               boxA[possibleA].trainID) {
                      if (boxB[b].trainID < boxB[possibleB].trainID) {
                        cost = timicost;
                        secondcost = pricecost;
                        possibleA = it->second.first;
                        // fir = boxA[it->second.first].trainID;
                        possibleB = b;
                        // sec = boxB[b].trainID;
                        MEET = it->first;
                      }
                    }
                  }
                }
              } else {
                if (pricecost < cost) {
                  cost = pricecost;
                  secondcost = timicost;
                  possibleA = it->second.first;
                  // fir = boxA[it->second.first].trainID;
                  possibleB = b;
                  // sec = boxB[b].trainID;
                  MEET = it->first;
                } else if (pricecost == cost) {
                  if (timicost < secondcost) {
                    cost = pricecost;
                    secondcost = timicost;
                    possibleA = it->second.first;
                    // fir = boxA[it->second.first].trainID;
                    possibleB = b;
                    // sec = boxB[b].trainID;
                    MEET = it->first;
                  } else if (timicost == secondcost) {
                    if (boxA[it->second.first].trainID <
                        boxA[possibleA].trainID) {
                      cost = pricecost;
                      secondcost = timicost;
                      possibleA = it->second.first;
                      // fir = boxA[it->second.first].trainID;
                      possibleB = b;
                      // sec = boxB[b].trainID;
                      MEET = it->first;
                    } else if (boxA[it->second.first].trainID ==
                               boxA[possibleA].trainID) {
                      if (boxB[b].trainID < boxB[possibleB].trainID) {
                        cost = pricecost;
                        secondcost = timicost;
                        possibleA = it->second.first;
                        // fir = boxA[it->second.first].trainID;
                        possibleB = b;
                        // sec = boxB[b].trainID;
                        MEET = it->first;
                      }
                    }
                  }
                }
              }
            }
            tm -= boxB[b].stopoverTimes[inp]._stopoverTimes;
          }
        }
      }
    }
  }
  if (possibleA == -1) {
    std::cout << "0\n";
    return false;
  }
  // std::cerr<<"cost"<<cost<<'\n';
  int miniseatA = 114514, miniseatB = 114514;
  bool start_count = false;
  int priceA = 0, priceB = 0;
  Seat seat;
  int begi = boxA[possibleA].startTime._startTime;
  Turn ta = seek_turn_from_station(boxA[possibleA], s, d);
  Seat_query_train(boxA[possibleA], seat, ta);
  int x1 = boxA[possibleA].startTime._startTime,
      x2 = boxB[possibleB].startTime._startTime;
  // std::cerr<<boxB[possibleB].trainID<<"
  // "<<boxB[possibleB].startTime._startTime<<"??\n";
  int durationA = 0, durationB = 0;
  for (int i = 0; i < boxA[possibleA].stationNum._stationnum; ++i) {
    if (boxA[possibleA].station[i] == s) {
      start_count = true;
      x1 += boxA[possibleA].stopoverTimes[i]._stopoverTimes;
      durationA += boxA[possibleA].travelTimes[i]._travelTimes;
      priceA += boxA[possibleA].price[i]._price;
      if (seat.seat[i] < miniseatA) {
        miniseatA = seat.seat[i];
      }
      continue;
    }
    if (boxA[possibleA].station[i] == MEET) {
      break;
    }
    if (start_count) {
      durationA += boxA[possibleA].stopoverTimes[i]._stopoverTimes +
                   boxA[possibleA].travelTimes[i]._travelTimes;
      priceA += boxA[possibleA].price[i]._price;
      if (seat.seat[i] < miniseatA) {
        miniseatA = seat.seat[i];
      }
    }
    if (!start_count) {
      x1 += boxA[possibleA].stopoverTimes[i]._stopoverTimes +
            boxA[possibleA].travelTimes[i]._travelTimes;
    }
  }
  for (int i = 0; i < boxB[possibleB].stationNum._stationnum; ++i) {
    if (boxB[possibleB].station[i] == t) {
      break;
    }
    x2 += boxB[possibleB].stopoverTimes[i]._stopoverTimes +
          boxB[possibleB].travelTimes[i]._travelTimes;
  }
  Turn tb;
  if (sort_type) {
    // std::cerr<<"x1 "<<x1<<"|cost "<<cost<<"|x2 "<<x2<<'\n';
    tb._turn = ta._turn + (x1 + cost - x2) / (24 * 60);
  } else {
    tb._turn = ta._turn + (x1 + secondcost - x2) / (24 * 60);
  }
  Seat_query_train(boxB[possibleB], seat, tb);
  start_count = false;
  for (int i = 0; i < boxB[possibleB].stationNum._stationnum; ++i) {
    if (boxB[possibleB].station[i] == MEET) {
      start_count = true;
      durationB += boxB[possibleB].travelTimes[i]._travelTimes;
      priceB += boxB[possibleB].price[i]._price;
      if (seat.seat[i] < miniseatB) {
        miniseatB = seat.seat[i];
      }
      continue;
    }
    if (boxB[possibleB].station[i] == t) {
      break;
    }
    if (start_count) {
      durationB += boxB[possibleB].stopoverTimes[i]._stopoverTimes +
                   boxB[possibleB].travelTimes[i]._travelTimes;
      priceB += boxB[possibleB].price[i]._price;
      if (seat.seat[i] < miniseatB) {
        miniseatB = seat.seat[i];
      }
    }
  }
  // std::cerr<<"durationB"<<durationB<<'\n';
  Time leave1, leave2, arrive1, arrive2;
  leave1._date = 600 + ta._turn + x1 / (24 * 60);
  MainTainDate(leave1._date);
  leave1._time = x1 % (24 * 60);
  arrive1._date = 600 + ta._turn + (x1 + durationA) / (24 * 60);
  MainTainDate(arrive1._date);
  arrive1._time = (x1 + durationA) % (24 * 60);

  leave2._date = 600 + tb._turn + (x2 - durationB) / (24 * 60);
  MainTainDate(leave2._date);
  leave2._time = (x2 - durationB) % (24 * 60);
  arrive2._date = 600 + tb._turn + (x2) / (24 * 60);
  MainTainDate(arrive2._date);
  arrive2._time = x2 % (24 * 60);
  std::cout << boxA[possibleA].trainID << ' ' << s << ' ' << leave1 << " -> "
            << MEET << ' ' << arrive1 << ' ' << priceA << ' ' << miniseatA
            << '\n';
  std::cout << boxB[possibleB].trainID << ' ' << MEET << ' ' << leave2 << " -> "
            << t << ' ' << arrive2 << ' ' << priceB << ' ' << miniseatB << '\n';
  return true;
}

void StationDate::clean() { database.clear(); }