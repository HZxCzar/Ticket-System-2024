#ifndef USER_HPP
#define USER_HPP
#include "Comparison.hpp"
#include "Database.hpp"
#include "Struct.hpp"
#include <cassert>
const string usfilename = "UserFile";
class User {
private:
  BPT<Username, UserInfo,39> database;

public:
  User() : database(usfilename) {}
  void add_user(Username &u, Password &p, Name &n, MailAddr &m,
                Privilege &g); //添加和c无关
  Privilege login(Username &u, Password &p);
  void query_profile(Username &c,Username &u,Privilege &c_p);
  //记得把登陆map的值也同步更改
  bool modify_profile(Username &c, Username &u, Password &p, Name &n,
                      MailAddr &m, Privilege &g,Privilege &c_p);
  UserInfo find(Username &c);
  void clean();
};

#endif