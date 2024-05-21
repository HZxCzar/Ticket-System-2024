#include "output.hpp"
using std::ostream;

ostream &operator<<(ostream &os, const UserInfo &s)
{
    os<<s.name._name<<" "<<s.mailAddr._mailAddr<<" "<<s.privilege._privilege;
    return os;
}

ostream &operator<<(ostream &os,const Username &s)
{
    os << s._username;
    return os;
}

ostream &operator<<(ostream &os,const Privilege &s)
{
    os<<s._privilege;
    return os;
}

ostream &operator<<(ostream &os,Time s)
{
    std::string month=std::to_string(s._date/100);
    std::string day=std::to_string(s._date%100);
    std::string hour=std::to_string(s._time/60);
    std::string minute=std::to_string(s._time%60);
    if(month.length()==1)
    {
        month="0"+month;
    }
    if(day.length()==1)
    {
        day="0"+day;
    }
    if(hour.length()==1)
    {
        hour="0"+hour;
    }
    if(minute.length()==1)
    {
        minute="0"+minute;
    }
    os<<month<<"-"<<day<<" "<<hour<<":"<<minute;
    return os;
}

ostream &operator<<(ostream &os,const Station &s)
{
    os<<s._station;
    return os;
}
ostream &operator<<(ostream &os,const TrainID &s)
{
    os<<s._trainID;
    return os;
}