#ifndef LOGIN_HPP
#define LOGIN_HPP
#include "Components/utility.hpp"
#include "Components/map.hpp"
#include "Components/utility.hpp"
#include "Comparison.hpp"

class LoginMap{
    private:
    sjtu::map<Username,Privilege> MyMap;
    public:
    LoginMap()=default;
    bool empty();
    void insert(Username &u,Privilege &p);
    void Modify_priv(Username &u,Privilege &g);
    bool out(Username &u);
    Privilege find(Username &c);
    void clean(){MyMap.clear();}
    // void show(){MyMap.Traverse();}
};

#endif // LOGIN_HPP