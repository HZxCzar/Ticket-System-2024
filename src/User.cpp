#include "User.hpp"

void User::add_user(Username &u, Password &p, Name &n, MailAddr &m,
                    Privilege &g) {
  UserInfo userinfo;
  userinfo.password = p;
  userinfo.name = n;
  userinfo.mailAddr = m;
  userinfo.privilege = g;
  if (database.Insert(u, userinfo)) {
    std::cout << "0\n";
  } else {
    std::cout << "-1\n";
  }
}

Privilege User::login(Username &u, Password &p) {
  UserInfo _set[2];
  Privilege res;
  res._privilege = -1;
  int size = 0;
  database.Find(u, _set, size);
  assert(size <= 1);
  if (size == 0) {
    return res;
  }
  if (_set[0].password == p) {
    res = _set[0].privilege;
  }
  return res;
}

void User::query_profile(Username &c, Username &u, Privilege &c_p) {
  UserInfo _set[2];
  Privilege res;
  res._privilege = -1;
  int size = 0;
  database.Find(u, _set, size);
  assert(size <= 1);
  if (size == 0) {
    std::cout << "-1\n";
    return;
  }
  if(c==u || c_p > _set[0].privilege)
  {
    std::cout<<u._username<<" "<<_set[0]<<'\n';
    return;
  }
  else
  {
    std::cout<<"-1\n";
    return;
  }
}

bool User::modify_profile(Username &c, Username &u, Password &p, Name &n, MailAddr &m, Privilege &g, Privilege &c_p)
{
  UserInfo _set[2];
  Privilege res;
  res._privilege = -1;
  int size = 0;
  database.Find(u, _set, size);
  assert(size <= 1);
  if (size == 0) {
    std::cout << "-1\n";
    return false;
  }
  if(c==u || c_p > _set[0].privilege)
  {
    if(p._password[0]!='\0')
    {
        _set[0].password=p;
    }
    if(n._name[0]!='\0')
    {
        _set[0].name=n;
    }
    if(m._mailAddr[0]!='\0')
    {
        _set[0].mailAddr=m;
    }
    if(g._privilege!=-1)
    {
        _set[0].privilege=g;
    }
    std::cout<<u._username<<" "<<_set[0]<<'\n';
    database.Modify(u,_set[0]);
    return true;
  }
  else
  {
    std::cout<<"-1\n";
    return false;
  }
}

void User::clean()
{
  database.clear();
}