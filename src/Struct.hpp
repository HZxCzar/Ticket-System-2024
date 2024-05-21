#ifndef STRUCT_HPP
#define STRUCT_HPP
#include <iostream>
//User.hpp
struct Username{
    char _username[21];
};
struct Password{
    char _password[31];
};
struct Name{
    char _name[16];
};
struct MailAddr{
    char _mailAddr[31];
};
struct Privilege{
    int _privilege;
};
struct UserInfo{
    Password password;
    Name name;
    MailAddr mailAddr;
    Privilege privilege;
};



//时间mm-dd hh-mm
struct Time{
    int _date;
    int _time;
};
//第几趟车
struct Turn{
    int _turn;
};
//站名
struct Station{
    char _station[31];
};

struct Date{
    int _date;
};
struct Seat{
    int seat[101];
};

//Train.hpp

struct TrainID{
    char _trainID[21];
};
struct StationNum{
    int _stationnum;
};
struct SeatNum{
    int _seatnum;
};
struct Prices{
    int _price;
};
struct StartTime{
    int _startTime;
};
struct TravelTimes{
    int _travelTimes;
};
struct StopoverTimes{
    int _stopoverTimes;
};
struct SaleDate{
    // char _beg[5];
    // char _end[5];
    Turn beg;
    Turn end;
};
struct Type{
    char _Type;
};

//StationFile
struct Box{
    int pos;
    int SeatBlockPos[101];
    TrainID trainID;
    StationNum stationNum;
    SeatNum seatnum;
    Station station[101];
    Prices price[101];
    StartTime startTime;
    TravelTimes travelTimes[101];
    StopoverTimes stopoverTimes[101];
};

struct Train{
    // TrainID trainID;
    // StationNum stationNum;
    // Station stations[101];
    // StationBox box;
    int box_pos;
    // SeatNum seatnum;
    // Prices prices;
    // StartTime startTime;
    // TravelTimes travelTimes;
    // StopoverTimes stopoverTimes;
    SaleDate saleDate;
    Type type;
    bool released;
};


//station.hpp
//key Station
//value TrainID

//Queue.hpp
//queue的key为trainID
struct QueKey{
    TrainID trainID;
    Turn turn;
};
//value 为
struct QueVal{
    int seq;
    Username username;
    Station begStation;
    Station endStation;
    int ticketNum;
};
//que和order共用seq,可采用操作日志编号

//Order.hpp
//key username
//value
enum Status{
    success,pending,refunded
};
struct OrderVal{
    int seq;
    Status status;
    TrainID trainID;
    Station from;
    Time leaving_time;
    Station to;
    Time arriving_time;
    int cost;
    int ticketNum;
};


// //RemSeat
// struct SeatKey{
//     int box_pos;
//     Turn turn;
// };

// struct SeatVal{
//     Turn turn;
//     int availaSeat[101];
// };

#endif