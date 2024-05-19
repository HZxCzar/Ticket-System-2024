#include "structure/Login.cpp"
#include "structure/Login.hpp"
// #include "structure/Order.hpp"
// #include "structure/Order.cpp"
// #include "structure/Queue.hpp"
// #include "structure/Queue.cpp"
#include "structure/Station.cpp"
#include "structure/Station.hpp"
#include "structure/Struct.hpp"
#include "structure/Tools/TokenScanner.cpp"
#include "structure/Tools/TokenScanner.hpp"
#include "structure/Train.cpp"
#include "structure/Train.hpp"
#include "structure/User.cpp"
#include "structure/User.hpp"
#include <cstdio>
#include <iostream>
#include <string>
// std::ifstream in("input.in");
// std::ofstream out("output.out");
void run() {
  std::fstream check;
  bool first = true;
  check.open("check", std::ios::in | std::ios::out);
  int point = check.tellp();
  if (point == -1) {
    check.close();
    check.open("check", std::ios::out);
    check.close();
    check.open("check", std::ios::out | std::ios::in | std::ios::ate);
    check.seekp(0);
    check.write(reinterpret_cast<char *>(&first), sizeof(bool));
    check.close();
  }
  check.seekg(0);
  check.read(reinterpret_cast<char *>(&first), sizeof(bool));
  // std::<<first<<'\n';
  std::string tokens[101];
  TokenScanner tokenscanner;
  LoginMap loginmap;
  User user;
  TrainDate traindate;
  StationDate stationdate;
  int size = 0;
  Username c, u;
  Password p;
  Name n;
  MailAddr m;
  Privilege g;
  TrainID trainid;
  StationNum stationnum;
  SeatNum seatnum;
  Station stations[101];
  Prices prices[101];
  StartTime starttime;
  TravelTimes traveltimes[101];
  StopoverTimes stopovertimes[101];
  SaleDate saledate;
  Type type;
  Date date;
  bool sort_type;
  // for (int pos = 0; pos < rang; ++pos) {
  while (true) {
    tokenscanner.ReadLine(tokens, size);
    // std::<<tokens[0]<<'\n';
    if (tokens[1] == "add_user") {
      if (first) {
        if (tokenscanner.first_user(tokens, size, u, p, n, m)) {
          g._privilege = 10;
          user.add_user(u, p, n, m, g);
          // std::<<"adfirst\n";
          first = false;
        } else {
          std::cout << "-1\n";
        }
      } else {
        if (tokenscanner.add_user(tokens, size, c, u, p, n, m, g)) {
          Privilege tmp;
          // std::cout<<c<<' ';
          tmp = loginmap.find(c);
          if (tmp._privilege != -1 && g < tmp) {
            user.add_user(u, p, n, m, g);
          } else {
            std::cout << "-1\n";
          }
        }
      }
    } else if (tokens[1] == "login") {
      if (tokenscanner.login(tokens, size, u, p)) {
        Privilege tmp;
        tmp = loginmap.find(u);
        if (tmp._privilege != -1) {
          std::cout << "-1\n";
        } else {
          tmp = user.login(u, p);
          if (tmp._privilege != -1) {
            loginmap.insert(u, tmp);
            std::cout << "0\n";
          } else {
            std::cout << "-1\n";
          }
        }
      } else {
        std::cout << "-1\n";
      }
    } else if (tokens[1] == "logout") {
      if (tokenscanner.logout(tokens, size, u)) {
        Privilege tmp;
        tmp = loginmap.find(u);
        if (tmp._privilege != -1) {
          loginmap.out(u);
          std::cout << "0\n";
        } else {
          std::cout << "-1\n";
        }
      } else {
        std::cout << "-1\n";
      }
    } else if (tokens[1] == "query_profile") {
      if (tokenscanner.query_profile(tokens, size, c, u)) {
        Privilege tmp = loginmap.find(c);
        if (tmp._privilege != -1) {
          user.query_profile(c, u, tmp);
        } else {
          std::cout << "-1\n";
        }
      } else {
        std::cout << "-1\n";
      }
    } else if (tokens[1] == "modify_profile") {
      if (tokenscanner.modify_profile(tokens, size, c, u, p, n, m, g)) {
        Privilege tmp = loginmap.find(c);
        if (tmp._privilege != -1 && tmp > g) {
          if (user.modify_profile(c, u, p, n, m, g, tmp)) {
            Privilege modif = loginmap.find(u);
            if (g._privilege != -1) {
              loginmap.Modify_priv(u, g);
            }
          }
        } else {
          std::cout << "-1\n";
        }
      } else {
        std::cout << "-1\n";
      }
    } else if (tokens[1] == "add_train") {
      // std::<<tokens[0]<<'\n';
      if (tokenscanner.add_train(tokens, size, trainid, stationnum, seatnum,
                                 stations, prices, starttime, traveltimes,
                                 stopovertimes, saledate, type)) {

        if (traindate.add_train(trainid, stationnum, seatnum, stations, prices,
                                starttime, traveltimes, stopovertimes, saledate,
                                type)) {
          std::cout << "0\n";
        } else {
          std::cout << "-1\n";
        }
      } else {
        std::cout << "-11\n";
      }
    } else if (tokens[1] == "delete_train") {
      if (tokenscanner.delete_train(tokens, size, trainid)) {
        Train output;
        if (traindate.delete_train(trainid, output)) {
          std::cout << "0\n";
        } else {
          std::cout << "-1\n";
        }
      } else {
        std::cout << "-1\n";
      }
    } else if (tokens[1] == "release_train") {
      if (tokenscanner.release_train(tokens, size, trainid)) {
        Train output;
        if (traindate.release_train(trainid, output)) {
          stationdate.release_train(output);
          std::cout << "0\n";
        } else {
          std::cout << "-1\n";
        }
      } else {
        std::cout << "-1\n";
      }
    } else if (tokens[1] == "query_train") {
      if (tokenscanner.query_train(tokens, size, trainid, date)) {
        Train output;
        if (!traindate.query_train(trainid, date, output)) {
          std::cout << "-1\n";
        }
      } else {
        std::cout << "-1\n";
      }
    } else if (tokens[1] == "query_ticket") {
      // std::<<tokens[0]<<'\n';
      Station s, t;
      if (tokenscanner.query_ticket(tokens, size, s, t, date, sort_type)) {
        stationdate.query_ticket(s, t, date, sort_type, tokens[0]);
      } else {
        std::cout << "not correct 0\n";
      }
    } else if (tokens[1] == "query_transfer") {
      Station s, t;
      // std::<<tokens[0]<<'\n';
      if (tokenscanner.query_transfer(tokens, size, s, t, date, sort_type)) {
        // if(sort_type==true)std::<<"time";
        stationdate.query_transfer(s, t, date, sort_type);
      } else {
        std::cout << "0\n";
      }
    } else if (tokens[1] == "buy_ticket") {
      int num, seq;
      Station f, t;
      bool bp = false;
      // std::<<tokens[0]<<'\n';
      if (tokenscanner.buy_ticket(tokens, size, u, trainid, date, num, f, t, bp,
                                  seq)) {
        Privilege tmp = loginmap.find(u);
        if (tmp._privilege != -1) {
          traindate.buy_ticket(u, trainid, date, num, f, t, bp, seq);
        } else {
          std::cout << "-1\n";
        }
      } else {
        std::cout << "not correct\n";
      }
    } else if (tokens[1] == "query_order") {
      if (tokenscanner.query_order(tokens, size, u)) {
        // std::<<tokens[0]<<"[query_order]"<<'\n';
        Privilege tmp = loginmap.find(u);
        if (tmp._privilege != -1) {
          traindate.query_order(u);
        } else {
          std::cout << "-1\n";
        }
      } else {
        std::cout << "not correct\n";
      }
    } else if (tokens[1] == "refund_ticket") {
      int num = 1;
      if (tokenscanner.refund_ticket(tokens, size, u, num)) {
        // std::<<tokens[0]<<'\n';
        Privilege tmp = loginmap.find(u);
        if (tmp._privilege != -1) {
          if (traindate.refund_ticket(u, num, tokens[0])) {
            std::cout << "0\n";
          } else {
            std::cout << "-1\n";
          }
        } else {
          std::cout << "-1\n";
        }
      }
    } else if (tokens[1] == "clean") {
      if (tokenscanner.clean(tokens, size)) {
        loginmap.clean();
        user.clean();
        traindate.clean();
        stationdate.clean();
        first = true;
        std::cout << "0\n";
      } else {
        std::cout << "not correct\n";
      }
    } else if (tokens[1] == "exit") {
      if (tokenscanner.exit(tokens, size)) {
        check.open("check", std::ios::out | std::ios::in | std::ios::ate);
        check.seekp(0);
        // std::<<first<<'\n';
        check.write(reinterpret_cast<char *>(&first), sizeof(bool));
        check.close();
        std::cout << "bye\n";
        // loginmap.show();
        return;
      } else {
        std::cout << "not correct\n";
      }
    } else {
      std::cout << tokens[0] << "|" << tokens[1] << '\n';
      std::cout << "man, what can I say! Out!\n";
    }
  }
  return;
}

int main() {
  // freopen("input.in", "r", stdin);
  // freopen("output.out", "w", stdout);
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  run();
  return 0;
}