#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "../Box.hpp"
#include "../Comparison.hpp"
#include "../Struct.hpp"
#include <cassert>
#include <string>

inline Turn MakeTurn(Date &d) {
  Turn duration;
  duration._turn = d._date%100;
  if (d._date/100 == 6) {
    return duration;
  } else if (d._date/100 == 7) {
    duration._turn += 30;
  } else {
    assert(d._date/100 == 8);
    duration._turn += 30 + 31;
  }
  return duration;
}

inline std::string BackDay(Date d)
{
  std::string res;
  std::string month=std::to_string(d._date/100);
  std::string day=std::to_string(d._date%100);
  if(month.length()==1)
  {
    month="0"+month;
  }
  if(day.length()==1)
  {
    day="0"+day;
  }
  res=month+"-"+day;
  return res;
}

inline std::string BackTime(int min)
{
  std::string hour=std::to_string(min/60);
  std::string minute=std::to_string(min%60);
  if(hour.length()==1)
  {
    hour="0"+hour;
  }
  if(minute.length()==1)
  {
    minute="0"+minute;
  }
  std::string res=hour+":"+minute;
  return res;
}

inline bool legal_date(SaleDate &sl, Box &t, Station &s, Date &d) {
  // std::<<d._date<<'\n';
  Turn turn = MakeTurn(d);
  // std::<<turn._turn<<'\n';
  int add, tm = t.startTime._startTime;
  for (int i = 0; i < t.stationNum._stationnum; ++i) {
    if (t.station[i] == s) {
      tm += t.stopoverTimes[i]._stopoverTimes;
      break;
    }
    tm += t.stopoverTimes[i]._stopoverTimes + t.travelTimes[i]._travelTimes;
  }
  add = tm / (24 * 60);
  // std::<<sl.beg._turn<<' '<<sl.end._turn<<'\n';
  return turn._turn >= sl.beg._turn + add && turn._turn <= sl.end._turn + add;
}

inline bool check_station_sequence(Box &box, Station &s, Station &t) {
  bool tag = false;
  for (int pos = 0; pos < box.stationNum._stationnum; ++pos) {
    // std::<<box.station[pos]._station<<' ';
    if (box.station[pos] == s) {
      tag = true;
    } else if (box.station[pos] == t) {
      return tag;
    }
  }
  return false;
}
inline void MySort(int *tag, int *index, Box *box, int l, int r) {
  int tmp;
  if (l >= r) {
    return;
  }
  if (l == r - 1) {
    if (tag[index[l]] > tag[index[r]] ||
        (tag[index[l]] == tag[index[r]] && box[index[l]].trainID > box[index[r]].trainID)) {
      tmp = index[r];
      index[r] = index[l];
      index[l] = tmp;
    }
    return;
  }
  int mid = (l + r) >> 1;
  MySort(tag, index, box, l, mid);
  MySort(tag, index, box, mid + 1, r);
  int pos1 = l, pos2 = mid + 1, pos3 = 0;
  int sheet2[1001];
  while (pos1 <= mid && pos2 <= r) {
    if (tag[index[pos1]] < tag[index[pos2]]) {
      sheet2[pos3++] = index[pos1++];
    } else if (tag[index[pos1]] > tag[index[pos2]]) {
      sheet2[pos3++] = index[pos2++];
    } else {
      if (box[index[pos1]].trainID < box[index[pos2]].trainID) {
        sheet2[pos3++] = index[pos1++];
      } else {
        sheet2[pos3++] = index[pos2++];
      }
    }
  }
  assert(pos3 <= r - l + 1);
  while (pos1 <= mid) {
    sheet2[pos3++] = index[pos1++];
  }
  while (pos2 <= r) {
    sheet2[pos3++] = index[pos2++];
  }
  assert(l + pos3 - 1 == r);
  for (int i = 0; i < pos3; ++i) {
    index[l + i] = sheet2[i];
  }
}

inline void MainTainDate(int &d)
{
  // std::<<d._date<<'\n';
  if(d/100==6 && d%100>30)
  {
    int tmp=d-630;
    d=700+tmp;
  }
  if(d/100==7 && d%100>31)
  {
    int tmp=d-731;
    d=800+tmp;
  }
  if(d/100==8 && d%100>31)
  {
    int tmp=d-831;
    d=900+tmp;
  }
}
inline Turn seek_turn_from_station(Box &box,Station &s,Date &d)
{
  int time=box.startTime._startTime;
  for(int i=0;i<box.stationNum._stationnum;++i)
  {
    if(box.station[i]==s)
    {
      time+=box.stopoverTimes[i]._stopoverTimes;
      break;
    }
    time+=box.stopoverTimes[i]._stopoverTimes+box.travelTimes[i]._travelTimes;
  }
  Turn res=MakeTurn(d);
  // std::<<"time"<<time<<"|turn"<<res._turn<<'\n';
  res._turn-=time/(24*60);
  // std::<<"turn2"<<res._turn<<'\n';
  return res;
}

#endif // FUNCTION_HPP