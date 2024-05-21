#include "Login.hpp"

bool LoginMap::empty() { return MyMap.empty(); }

void LoginMap::insert(Username &u, Privilege &p) {
  MyMap.insert(sjtu::pair<Username, Privilege>(u, p));
}

bool LoginMap::out(Username &u) {
  sjtu::map<Username, Privilege>::iterator it = MyMap.find(u);
  if (it != MyMap.end()) {
    MyMap.erase(it);
    return true;
  }
  return false;
}

Privilege LoginMap::find(Username &c) {
  Privilege res;
  res._privilege = -1; //-1未找到
  if (MyMap.count(c)) {
    res = MyMap.at(c);
  }
  return res;
}

void LoginMap::Modify_priv(Username &u, Privilege &g) {
  sjtu::map<Username, Privilege>::iterator it = MyMap.find(u);
  if (it != MyMap.end()) {
    it->second = g;
  }
}