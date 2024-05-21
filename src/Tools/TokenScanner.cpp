#include "TokenScanner.hpp"
#include "function.hpp"
#include <cctype>
#include <cstring>
#include <sstream>
#include <string>
#pragma region
void TokenScanner::ReadLine(string *tokens, int &size) {
  std::getline(std::cin, line);
  size = 0;
  int beg = 0, end = line.size() - 1;
  while (line[beg] == ' ') {
    beg++;
  }
  while (line[end] == ' ') {
    end--;
  }
  token = "";
  for (int i = beg; i <= end; ++i) {
    if (line[i] == ' ') {
      tokens[size++] = token;
      token = "";
    } else {
      token += line[i];
    }
  }
  tokens[size++] = token;
  token = "";
  std::cout << tokens[0] << ' ';
}

int TokenScanner::countChineseCharacters(const std::string &str) {
  int count = 0;
  for (size_t i = 0; i < str.size(); ++i) {
    char c = str[i];
    if ((c & 0xE0) ==
        0xE0) { // 检查前三位是否为111，如果是，说明这是一个三字节的UTF-8字符
      ++count;
      i += 2; // 跳过接下来的两个字节
    }
  }
  return count;
}

Username TokenScanner::cUsername(string token, bool &flag) {
  Username ans;
  if (token.size() <= 0 || token.size() > 20) {
    flag = false;
  }
  if (!flag)
    return ans;
  for (int i = 0; i < token.size(); ++i) {
    ans._username[i] = token[i];
    if (!std::isalnum(token[i]) && !(token[i] == '_')) {
      flag = false;
      break;
    }
  }
  ans._username[token.size()] = '\0';
  return ans;
}

Password TokenScanner::cPassword(string token, bool &flag) {
  Password ans;
  if (token.size() <= 0 || token.size() > 30) {
    flag = false;
  }
  if (!flag)
    return ans;
  for (int i = 0; i < token.size(); ++i) {
    ans._password[i] = token[i];
    if (!std::isprint(token[i])) {
      flag = false;
      break;
    }
  }
  ans._password[token.size()] = '\0';
  return ans;
}

Name TokenScanner::cName(string token, bool &flag) {
  Name ans;
  int count = countChineseCharacters(token);
  if (count < 2 || count > 5) {
    flag = false;
    return ans;
  }
  std::strcpy(ans._name, token.c_str());
  return ans;
}

MailAddr TokenScanner::cMailAddr(string token, bool &flag) {
  MailAddr ans;

  // 检查长度是否超过30
  if (token.size() > 30) {
    flag = false;
    return ans;
  }

  for (char c : token) {
    if (!std::isdigit(c) && !std::isalpha(c) && c != '@' && c != '.') {
      flag = false;
      return ans;
    }
  }

  std::strcpy(ans._mailAddr, token.c_str());

  return ans;
}
// 
Privilege TokenScanner::cPrivilege(string token, bool &flag) {
  Privilege ans;
  int privilege = std::stoi(token, nullptr, 10);
  if (privilege < 0 || privilege > 10) {
    flag = false;
    return ans;
  }
  ans._privilege = privilege;
  return ans;
}

// Time TokenScanner::cTime(string token, bool &flag) { Time ans; }

// Turn TokenScanner::cTurn(string token, bool &flag) { Turn ans; }

void TokenScanner::cStation(string token, bool &flag, Station *s, int &size) {
  // 使用'|'分割token
  std::istringstream ss(token);
  std::string station;
  size=0;
  while (std::getline(ss, station, '|')) {
    // 检查每个车站名的长度是否超过10个汉字
    if (countChineseCharacters(station) > 10) {
      flag = false;
      return;
    }

    // 将车站名赋值给一个新的Station对象，并将这个对象添加到s数组中
    Station ans;
    std::strcpy(ans._station, station.c_str());
    s[size++] = ans;
  }
}

Date TokenScanner::cDate(string token, bool &flag) {
  Date ans;
  if (!flag || token.size() != 5 || token[2] != '-') {
    flag = false;
    return ans;
  }
  for (int i : {0, 1, 3, 4}) {
    if (!std::isdigit(token[i])) {
      flag = false;
      return ans;
    }
  }
  string month = token.substr(0, 2);
  string day = token.substr(3, 2);

  if (month < "06" || month > "08") {
    flag = false;
    return ans;
  }

  if (month == "06" && (day < "01" || day > "30")) {
    flag = false;
    return ans;
  }

  if ((month == "07" || month == "08") && (day < "01" || day > "31")) {
    flag = false;
    return ans;
  }
  ans._date = 1000 * (token[0] - '0') + 100 * (token[1] - '0') +
              10 * (token[3] - '0') + (token[4] - '0');
  return ans;
}

TrainID TokenScanner::cTrainID(string token, bool &flag) {
  TrainID ans;

  // 检查长度是否超过20
  // if (token.size() > 20) {
  //   flag = false;
  //   return ans;
  // }

  // // 检查是否由字母开头
  // if (!std::isalpha(token[0])) {
  //   flag = false;
  //   return ans;
  // }

  // 检查是否由字母、数字和下划线组成
  // for (char c : token) {
  //   if (!std::isalnum(c) && c != '_') {
  //     flag = false;
  //     return ans;
  //   }
  // }

  // 将token赋值给ans._trainID
  std::strcpy(ans._trainID, token.c_str());

  return ans;
}

StationNum TokenScanner::cStationNum(string token, bool &flag) {
  StationNum ans;

  // 将token转换为整数
  int num = std::stoi(token);

  // 检查num是否在2到100之间
  if (num < 2 || num > 100) {
    flag = false;
    return ans;
  }

  // 将num赋值给ans._stationNum
  ans._stationnum = num;

  return ans;
}

SeatNum TokenScanner::cSeatNum(string token, bool &flag) {
  SeatNum ans;

  // 将token转换为整数
  int num = std::stoi(token);

  // 检查num是否不超过100000
  if (num > 100000) {
    flag = false;
    return ans;
  }

  // 将num赋值给ans._seatNum
  ans._seatnum = num;

  return ans;
}

void TokenScanner::cPrices(string token, bool &flag, Prices *set, int &size) {
  std::stringstream ss(token);
  string item;
  int i = 0;
  size=0;
  while (std::getline(ss, item, '|')) {
    int price = std::stoi(item);

    if (price > 100000) {
      flag = false;
      return;
    }

    set[i++]._price = price;
  }

  size = i;
}

StartTime TokenScanner::cStartTime(string token, bool &flag) {
  StartTime ans;

  // 检查token的长度是否为5，以及第2个字符是否为':'
  if (token.size() != 5 || token[2] != ':') {
    flag = false;
    return ans;
  }

  // 将token的小时部分和分钟部分转换为整数
  int hour = std::stoi(token.substr(0, 2));
  int minute = std::stoi(token.substr(3, 2));

  // 检查hour和minute是否在合法范围内
  if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
    flag = false;
    return ans;
  }

  // 将时间转换为分钟数并赋值给ans._startTime
  ans._startTime = hour * 60 + minute;

  return ans;
}

void TokenScanner::cTravelTimes(string token, bool &flag, TravelTimes *set,
                                int &size) {
  std::stringstream ss(token);
  string item;
  int i = 0;
  size=0;
  while (std::getline(ss, item, '|')) {
    int time = std::stoi(item);

    if (time > 10000) {
      flag = false;
      return;
    }

    set[i++]._travelTimes = time;
  }

  size = i;
}

void TokenScanner::cStopoverTimes(string token, bool &flag, StopoverTimes *set,
                                  int &size) {
  std::stringstream ss(token);
  string item;
  int i = 0;
  size=0;
  while (std::getline(ss, item, '|')) {

    int time = std::stoi(item);

    if (time > 10000) {
      flag = false;
      return;
    }

    set[i++]._stopoverTimes = time;
  }

  size = i;
}

SaleDate TokenScanner::cSaleDate(string token, bool &flag) {
  SaleDate ans;

  // 检查token的长度是否为11，以及第2个和第6个字符是否为'-'
  if (token.size() != 11 || token[2] != '-' || token[5] != '|' ||
      token[8] != '-') {
    flag = false;
    return ans;
  }

  // 将token的月份部分和日期部分转换为整数
  int startMonth = std::stoi(token.substr(0, 2));
  int startDay = std::stoi(token.substr(3, 2));
  int endMonth = std::stoi(token.substr(6, 2));
  int endDay = std::stoi(token.substr(9, 2));

  // 检查月份是否在合法范围内
  if (startMonth < 6 || startMonth > 8 || endMonth < 6 || endMonth > 8) {
    flag = false;
    return ans;
  }

  // 检查日期是否在合法范围内
  if ((startMonth == 6 && startDay > 30) ||
      (startMonth == 7 && startDay > 31) ||
      (startMonth == 8 && startDay > 31) || (endMonth == 6 && endDay > 30) ||
      (endMonth == 7 && endDay > 31) || (endMonth == 8 && endDay > 31) ||
      startDay < 1 || endDay < 1) {
    flag = false;
    return ans;
  }

  // 将日期赋值给ans._start和ans._end
  Date beg, end;
  // std::<<startMonth<<' '<<startDay<<'\n';
  // std::<<endMonth<<' '<<endDay<<'\n';
  beg._date = startMonth * 100 + startDay;
  end._date = endMonth * 100 + endDay;
  ans.beg = MakeTurn(beg);
  ans.end = MakeTurn(end);
  return ans;
}

Type TokenScanner::cType(string token, bool &flag) {
  Type ans;

  // 检查token的长度是否为1，以及是否为大写字母
  if (token.size() != 1 || !isupper(token[0])) {
    flag = false;
    return ans;
  }

  // 将token赋值给ans
  ans._Type = token[0];

  return ans;
}

int TokenScanner::cseq(string token, bool &flag) {
  int ans;
  // 检查token的长度是否大于等于3，以及第一个和最后一个字符是否为'['和']'
  if (token.size() < 3 || token[0] != '[' || token[token.size() - 1] != ']') {
    flag = false;
    return ans;
  }

  // 提取出seq，并尝试将其转换为一个整数
  string seq = token.substr(1, token.size() - 2);
  try {
    ans = std::stoi(seq);
  } catch (std::invalid_argument &) {
    flag = false;
    return ans;
  }
  return ans;
}
#pragma endregion
bool TokenScanner::first_user(string *tokens, int size, Username &u,
                              Password &p, Name &n, MailAddr &m) {
  bool flag = true;
  int seq = cseq(tokens[0], flag);
  // if (size != 10) {
  //   return false;
  // }
  for (int pos = 2; pos < size; pos += 2) {
    if (tokens[pos] == "-u") {
      u = cUsername(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-p") {
      p = cPassword(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-n") {
      n = cName(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-m") {
      m = cMailAddr(tokens[pos + 1], flag);
    }
    if (flag == false) {
      // std:: << tokens[pos] << '\n';
    }
  }
  // if (tokens[2] != "-u") {
  //   return false;
  // }
  // u = cUsername(tokens[3], flag);
  // if (tokens[4] != "-p") {
  //   return false;
  // }
  // p = cPassword(tokens[5], flag);
  // if (tokens[6] != "-n") {
  //   return false;
  // }
  // n = cName(tokens[7], flag);
  // if (tokens[8] != "-m") {
  //   return false;
  // }
  // m = cMailAddr(tokens[9], flag);
  return flag;
}

bool TokenScanner::add_user(string *tokens, int size, Username &c, Username &u,
                            Password &p, Name &n, MailAddr &m, Privilege &g) {
  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 14) {
    return false;
  }
  for (int pos = 2; pos < size; pos += 2) {
    if (tokens[pos] == "-c") {
      c = cUsername(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-u") {
      u = cUsername(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-p") {
      p = cPassword(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-n") {
      n = cName(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-m") {
      m = cMailAddr(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-g") {
      g = cPrivilege(tokens[pos + 1], flag);
    }
  }
  // if (tokens[2] != "-c") {
  //   return false;
  // }
  // c = cUsername(tokens[3], flag);
  // if (tokens[4] != "-u") {
  //   return false;
  // }
  // u = cUsername(tokens[5], flag);
  // if (tokens[6] != "-p") {
  //   return false;
  // }
  // p = cPassword(tokens[7], flag);
  // if (tokens[8] != "-n") {
  //   return false;
  // }
  // n = cName(tokens[9], flag);
  // if (tokens[10] != "-m") {
  //   return false;
  // }
  // m = cMailAddr(tokens[11], flag);
  // if (tokens[12] != "-g") {
  //   return false;
  // }
  // g = cPrivilege(tokens[13], flag);
  return flag;
}

bool TokenScanner::login(string *tokens, int size, Username &u, Password &p) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 6) {
    return false;
  }
  for (int pos = 2; pos < size; pos += 2) {
    if (tokens[pos] == "-u") {
      u = cUsername(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-p") {
      p = cPassword(tokens[pos + 1], flag);
    }
  }
  return flag;
}

bool TokenScanner::logout(string *tokens, int size, Username &u) {
  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 4) {
    return false;
  }
  if (tokens[2] != "-u") {
    return false;
  }
  u = cUsername(tokens[3], flag);
  return flag;
}

bool TokenScanner::query_profile(string *tokens, int size, Username &c,
                                 Username &u) {
  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 6) {
    return false;
  }
  for (int pos = 2; pos < size; pos += 2) {
    if (tokens[pos] == "-c") {
      c = cUsername(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-u") {
      u = cUsername(tokens[pos + 1], flag);
    }
  }
  return flag;
}

bool TokenScanner::modify_profile(string *tokens, int size, Username &c,
                                  Username &u, Password &p, Name &n,
                                  MailAddr &m, Privilege &g) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  // if (tokens[2] != "-c") {
  //   return false;
  // }
  // c = cUsername(tokens[3], flag);
  // if (tokens[4] != "-u") {
  //   return false;
  // }
  // u = cUsername(tokens[5], flag);
  p._password[0] = '\0';
  n._name[0] = '\0';
  m._mailAddr[0] = '\0';
  g._privilege = -1;
  int pos = 2;
  bool flagp = false, flagn = false, flagm = false, flagg = false;
  while (pos < size) {
    if (pos + 1 == size)
    {
      return false;
    }
    if(tokens[pos]=="-c")
    {
      c = cUsername(tokens[pos+1], flag);
    }
    else if(tokens[pos]=="-u")
    {
      u = cUsername(tokens[pos+1], flag);
    }
    else if (tokens[pos] == "-p") {
      if (flagp)
        return false;
      p = cPassword(tokens[pos + 1], flag);
      flagp = true;
    } else if (tokens[pos] == "-n") {
      if (flagn)
        return false;
      n = cName(tokens[pos + 1], flag);
      flagn = true;
    } else if (tokens[pos] == "-m") {
      if (flagm)
        return false;
      m = cMailAddr(tokens[pos + 1], flag);
      flagm = true;
    } else if (tokens[pos] == "-g") {
      if (flagg)
        return false;
      g = cPrivilege(tokens[pos + 1], flag);
      flagg = true;
    } else {
      return false;
    }
    pos += 2;
  }
  return flag;
}

bool TokenScanner::add_train(string *tokens, int size, TrainID &i,
                             StationNum &n, SeatNum &m, Station *s, Prices *p,
                             StartTime &x, TravelTimes *t, StopoverTimes *o,
                             SaleDate &d, Type &y) {

  bool flag = true;
  if (size != 22) {
    return false;
  }
  int seq = cseq(tokens[0], flag);
  for (int pos = 2; pos < 22; pos += 2) {
    if (tokens[pos] == "-i") {
      i = cTrainID(tokens[pos + 1], flag);
      if(!flag)std::cout<<"-t"<<tokens[pos+1];
    }
    else if (tokens[pos] == "-n") {
      n = cStationNum(tokens[pos + 1], flag);
      if(!flag)std::cout<<"-n"<<tokens[pos+1];
    } else if (tokens[pos] == "-m") {
      m = cSeatNum(tokens[pos + 1], flag);
      if(!flag)std::cout<<"-m"<<tokens[pos+1];
    } else if (tokens[pos] == "-s") {
      cStation(tokens[pos + 1], flag, s, size);
      if(!flag)std::cout<<"-s"<<tokens[pos+1];
      // if (size != n._stationnum) {
      //   return false;
      // }
    } else if (tokens[pos] == "-p") {
      cPrices(tokens[pos + 1], flag, p, size);
      if(!flag)std::cout<<"-p"<<tokens[pos+1];
      // if (size != n._stationnum - 1) {
      //   return false;
      // }
    } else if (tokens[pos] == "-x") {
      x = cStartTime(tokens[pos + 1], flag);
      if(!flag)std::cout<<"-x"<<tokens[pos+1];
    }

    else if (tokens[pos] == "-t") {
      cTravelTimes(tokens[pos + 1], flag, t, size);
      if(!flag)std::cout<<"-t"<<tokens[pos+1];
      // if (size != n._stationnum - 1) {
      //   return false;
      // }
    }
    else if (tokens[pos] == "-o") {
      cStopoverTimes(tokens[pos+1], flag, o, size);
      if(!flag)std::cout<<"-o"<<tokens[pos+1];
      // if (size != n._stationnum - 2) {
      //   return false;
      // }
    }

    else if (tokens[pos] == "-d") {
      // std::<<tokens[pos+1]<<'\n';
      d = cSaleDate(tokens[pos + 1], flag);
      if(!flag)std::cout<<"-d"<<tokens[pos+1];
    }

    else if (tokens[pos] == "-y") {
      y = cType(tokens[pos + 1], flag);
      if(!flag)std::cout<<"-y"<<tokens[pos+1];
    }
  }
  return flag;
}

bool TokenScanner::delete_train(string *tokens, int size, TrainID &i) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 4) {
    return false;
  }
  if (tokens[2] != "-i") {
    return false;
  }
  i = cTrainID(tokens[3], flag);
  return flag;
}

bool TokenScanner::release_train(string *tokens, int size, TrainID &i) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 4) {
    return false;
  }
  if (tokens[2] != "-i") {
    return false;
  }
  i = cTrainID(tokens[3], flag);
  return flag;
}

bool TokenScanner::query_train(string *tokens, int size, TrainID &i, Date &d) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 6) {
    return false;
  }
  for (int pos = 2; pos < size; pos += 2) {
    if (tokens[pos] == "-i") {
      i = cTrainID(tokens[pos + 1], flag);
    }

    else if (tokens[pos] == "-d") {
      d = cDate(tokens[pos + 1], flag);
    }
  }
  return flag;
}

bool TokenScanner::query_ticket(string *tokens, int size, Station &s,
                                Station &t, Date &d, bool &sort_type) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 8 && size != 10) {
    return false;
  }
  Station set[2];
  int ssize = 0;
  for (int pos = 2; pos < size; pos += 2) {
    if (tokens[pos] == "-s") {
      ssize = 0;
      cStation(tokens[pos + 1], flag, set, ssize);
      if (ssize != 1) {
        return false;
      }
      s = set[0];
    }

    else if (tokens[pos] == "-t") {
      ssize = 0;
      cStation(tokens[pos + 1], flag, set, ssize);
      if (ssize != 1) {
        return false;
      }
      t = set[0];
    }

    else if (tokens[pos] == "-d") {
      d = cDate(tokens[pos + 1], flag);

    } else if (tokens[pos] == "-p") {
      if (tokens[pos + 1] == "time") {
        sort_type = true;
      } else if (tokens[pos + 1] == "cost") {
        sort_type = false;
      } else {
        flag = false;
      }
    }
  }
  if (size == 8) {
    sort_type = true;
    return flag;
  }
  return flag;
}

bool TokenScanner::query_transfer(string *tokens, int size, Station &s,
                                  Station &t, Date &d, bool &sort_type) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 8 && size != 10) {
    return false;
  }
  Station set[2];
  int ssize = 0;
  for (int pos = 2; pos < size; pos += 2) {
    if (tokens[pos] == "-s") {
      ssize = 0;
      cStation(tokens[pos + 1], flag, set, ssize);
      if (ssize != 1) {
        return false;
      }
      s = set[0];
    }

    else if (tokens[pos] == "-t") {
      ssize = 0;
      cStation(tokens[pos + 1], flag, set, ssize);
      if (ssize != 1) {
        return false;
      }
      t = set[0];
    }

    else if (tokens[pos] == "-d") {
      d = cDate(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-p") {
      if (tokens[pos + 1] == "time") {
        sort_type = true;
      } else if (tokens[pos + 1] == "cost") {
        sort_type = false;
      } else {
        flag = false;
      }
    }
  }
  if (size == 8) {
    sort_type = true;
    return flag;
  }
  return flag;
}

bool TokenScanner::buy_ticket(string *tokens, int size, Username &u, TrainID &i,
                              Date &d, int &n, Station &f, Station &t, bool &bp,
                              int &seq) {

  bool flag = true;
  seq = cseq(tokens[0], flag);
  if (size != 14 && size != 16) {
    return false;
  }
  for (int pos = 2; pos < size; pos += 2) {
    if (tokens[pos] == "-u") {
      u = cUsername(tokens[pos + 1], flag);
    }

    else if (tokens[pos] == "-i") {
      i = cTrainID(tokens[pos + 1], flag);
    }

    else if (tokens[pos] == "-d") {
      d = cDate(tokens[pos + 1], flag);
    }

    else if (tokens[pos] == "-n") {
      n = std::stoi(tokens[pos + 1]);
    }

    else if (tokens[pos] == "-f") {
      Station set[2];
      int ssize = 0;
      cStation(tokens[pos + 1], flag, set, ssize);
      if (ssize != 1) {
        return false;
      }
      f = set[0];
    } else if (tokens[pos] == "-t") {
      Station set[2];
      int ssize = 0;
      cStation(tokens[pos + 1], flag, set, ssize);
      if (ssize != 1) {
        return false;
      }
      t = set[0];
    } else if (tokens[pos] == "-q") {
      if (tokens[pos + 1] == "true") {
        bp = true;
      } else if (tokens[pos + 1] == "false") {
        bp = false;
      } else {
        flag = false;
      }
    }
  }
  if (size == 14) {
    bp = false;
    return flag;
  }
  return flag;
}

bool TokenScanner::query_order(string *tokens, int size, Username &u) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 4) {
    return false;
  }
  if (tokens[2] != "-u") {
    return false;
  }
  u = cUsername(tokens[3], flag);
  return flag;
}

bool TokenScanner::refund_ticket(string *tokens, int size, Username &u,
                                 int &num) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 4 && size != 6) {
    return false;
  }
  for (int pos = 2; pos < size; pos += 2) {
    if (tokens[pos] == "-u") {
      u = cUsername(tokens[pos + 1], flag);
    } else if (tokens[pos] == "-n") {
      num = std::stoi(tokens[pos + 1]);
    }
  }
  if (size == 4) {
    num = 1;
    return false;
  }
  return flag;
}

bool TokenScanner::clean(string *tokens, int size) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 2) {
    return false;
  }
  return flag;
}

bool TokenScanner::exit(string *tokens, int size) {

  bool flag = true;
  int seq = cseq(tokens[0], flag);
  if (size != 2) {
    return false;
  }
  return flag;
}