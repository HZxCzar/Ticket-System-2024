#ifndef TOKENSCANNER_HPP
#define TOKENSCANNER_HPP
#include <string>
#include <iostream>
#include "../Struct.hpp"
using std::string;

class TokenScanner{
    private:
    string token;
    string behind;
    string line;
    int countChineseCharacters(const std::string& str);

    Username cUsername(string token,bool &flag);
    Password cPassword(string token,bool &flag);
    Name cName(string token,bool &flag);
    MailAddr cMailAddr(string token,bool &flag);
    Privilege cPrivilege(string token,bool &flag);
    Time cTime(string token,bool &flag);
    Turn cTurn(string token,bool &flag);
    void cStation(string token,bool &flag,Station *s,int &size);
    Date cDate(string token,bool &flag);
    TrainID cTrainID(string token,bool &flag);
    StationNum cStationNum(string token,bool &flag);
    SeatNum cSeatNum(string token,bool &flag);
    void cPrices(string token,bool &flag,Prices *,int &size);
    StartTime cStartTime(string token,bool &flag);
    void cTravelTimes(string token,bool &flag,TravelTimes *,int &size);
    void cStopoverTimes(string token,bool &flag,StopoverTimes *,int &size);
    SaleDate cSaleDate(string token,bool &flag);
    Type cType(string token,bool &flag);
    int cseq(string token,bool &flag);

    public:
    TokenScanner(){}
    void ReadLine(string *tokens,int &size);
    bool first_user(string *tokens,int size,Username &u,Password &p,Name &n,MailAddr &m);
    bool add_user(string *tokens,int size,Username &c,Username &u,Password &p,Name &n,MailAddr &m,Privilege &g);
    bool login(string *tokens,int size,Username &u,Password &p);
    bool logout(string *tokens,int size,Username &u);
    bool query_profile(string *tokens,int size,Username &c,Username &u);
    bool modify_profile(string *tokens,int size,Username &c,Username &u,Password &p,Name &n,MailAddr &m,Privilege &g);
    bool add_train(string *tokens,int size,TrainID &i,StationNum &n,SeatNum &m,Station *s,Prices *p,StartTime &x,TravelTimes *t,StopoverTimes *o,SaleDate &d,Type &y);
    bool delete_train(string *tokens,int size,TrainID &i);
    bool release_train(string *tokens,int size,TrainID &i);
    bool query_train(string *tokens,int size,TrainID &i,Date &d);
    bool query_ticket(string *tokens,int size,Station &s,Station &t,Date &d,bool &sort_type);
    bool query_transfer(string *tokens,int size,Station &s,Station &t,Date &d,bool &sort_type);
    bool buy_ticket(string *tokens,int size,Username &u,TrainID &i,Date &d,int &n,Station &f,Station &t,bool &bp,int &seq);
    bool query_order(string *tokens,int size,Username &u);
    bool refund_ticket(string *tokens,int size,Username &u,int &num);
    bool clean(string *tokens,int size);
    bool exit(string *tokens,int size);
};

#endif // TOKENSCANNER_HPP