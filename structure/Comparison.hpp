#ifndef COMPARISON_HPP
#define COMPARISON_HPP

#include "Struct.hpp"
#include <cstddef>
#include <cstring>

// User
#pragma region
inline bool operator<(const Username &lhs, const Username &rhs) {
  return std::strcmp(lhs._username, rhs._username) < 0;
}
inline bool operator>(const Username &lhs, const Username &rhs) {
  return std::strcmp(lhs._username, rhs._username) > 0;
}

inline bool operator==(const Username &lhs, const Username &rhs) {
  return std::strcmp(lhs._username, rhs._username) == 0;
}

inline bool operator!=(const Username &lhs, const Username &rhs) {
  return std::strcmp(lhs._username, rhs._username) != 0;
}

inline bool operator<(const UserInfo &lhs, const UserInfo &rhs) {
  return lhs.privilege._privilege<rhs.privilege._privilege;
}
inline bool operator>(const UserInfo &lhs, const UserInfo &rhs) {
  return lhs.privilege._privilege>rhs.privilege._privilege;
}
inline bool operator==(const UserInfo &lhs, const UserInfo &rhs) {
  return lhs.privilege._privilege == rhs.privilege._privilege;
}
inline bool operator!=(const UserInfo &lhs, const UserInfo &rhs) {
  return lhs.privilege._privilege != rhs.privilege._privilege;
}


inline bool operator<(const Password &lhs, const Password &rhs) {
  return std::strcmp(lhs._password, rhs._password) < 0;
}
inline bool operator>(const Password &lhs, const Password &rhs) {
  return std::strcmp(lhs._password, rhs._password) > 0;
}

inline bool operator==(const Password &lhs, const Password &rhs) {
  return std::strcmp(lhs._password, rhs._password) == 0;
}

inline bool operator!=(const Password &lhs, const Password &rhs) {
  return std::strcmp(lhs._password, rhs._password) != 0;
}

inline bool operator<(const Name &lhs, const Name &rhs) {
  return std::strcmp(lhs._name, rhs._name) < 0;
}
inline bool operator>(const Name &lhs, const Name &rhs) {
  return std::strcmp(lhs._name, rhs._name) > 0;
}

inline bool operator==(const Name &lhs, const Name &rhs) {
  return std::strcmp(lhs._name, rhs._name) == 0;
}

inline bool operator!=(const Name &lhs, const Name &rhs) {
  return std::strcmp(lhs._name, rhs._name) != 0;
}

inline bool operator<(const MailAddr &lhs, const MailAddr &rhs) {
  return std::strcmp(lhs._mailAddr, rhs._mailAddr) < 0;
}
inline bool operator>(const MailAddr &lhs, const MailAddr &rhs) {
  return std::strcmp(lhs._mailAddr, rhs._mailAddr) > 0;
}

inline bool operator==(const MailAddr &lhs, const MailAddr &rhs) {
  return std::strcmp(lhs._mailAddr, rhs._mailAddr) == 0;
}

inline bool operator!=(const MailAddr &lhs, const MailAddr &rhs) {
  return std::strcmp(lhs._mailAddr, rhs._mailAddr) != 0;
}

inline bool operator<(const Privilege &lhs, const Privilege &rhs) {
  return lhs._privilege < rhs._privilege;
}

inline bool operator>(const Privilege &lhs, const Privilege &rhs) {
  return lhs._privilege > rhs._privilege;
}

inline bool operator==(const Privilege &lhs, const Privilege &rhs) {
  return lhs._privilege == rhs._privilege;
}

inline bool operator!=(const Privilege &lhs, const Privilege &rhs) {
  return lhs._privilege != rhs._privilege;
}

#pragma endregion

// TrainGene
#pragma region

inline bool operator<(const Turn &lhs, const Turn &rhs) {
  return lhs._turn < rhs._turn;
}
inline bool operator>(const Turn &lhs, const Turn &rhs) {
  return lhs._turn > rhs._turn;
}
inline bool operator==(const Turn &lhs, const Turn &rhs) {
  return lhs._turn == rhs._turn;
}
inline bool operator!=(const Turn &lhs, const Turn &rhs) {
  return lhs._turn != rhs._turn;
}

inline bool operator<(const Station &lhs, const Station &rhs) {
  return std::strcmp(lhs._station, rhs._station) < 0;
}
inline bool operator>(const Station &lhs, const Station &rhs) {
  return std::strcmp(lhs._station, rhs._station) > 0;
}

inline bool operator==(const Station &lhs, const Station &rhs) {
  return std::strcmp(lhs._station, rhs._station) == 0;
}

inline bool operator!=(const Station &lhs, const Station &rhs) {
  return std::strcmp(lhs._station, rhs._station) != 0;
}
#pragma endregion

// Train
#pragma region
inline bool operator<(const TrainID &lhs, const TrainID &rhs) {
  return std::strcmp(lhs._trainID, rhs._trainID) < 0;
}
inline bool operator>(const TrainID &lhs, const TrainID &rhs) {
  return std::strcmp(lhs._trainID, rhs._trainID) > 0;
}

inline bool operator==(const TrainID &lhs, const TrainID &rhs) {
  return std::strcmp(lhs._trainID, rhs._trainID) == 0;
}

inline bool operator!=(const TrainID &lhs, const TrainID &rhs) {
  return std::strcmp(lhs._trainID, rhs._trainID) != 0;
}
#pragma endregion

//station
#pragma region 
inline bool operator<(const Train &lhs, const Train &rhs) {
  return lhs.box_pos < rhs.box_pos;
}

inline bool operator>(const Train &lhs, const Train &rhs) {
  return lhs.box_pos > rhs.box_pos;
}

inline bool operator==(const Train &lhs, const Train &rhs) {
  return lhs.box_pos == rhs.box_pos;
}

inline bool operator!=(const Train &lhs, const Train &rhs) {
  return lhs.box_pos != rhs.box_pos;
}
#pragma endregion

// Queue
#pragma region
inline bool operator<(const QueKey &lhs, const QueKey &rhs) {
  if (lhs.trainID < rhs.trainID) {
    return true;
  } else {
    if (lhs.trainID == rhs.trainID) {
      return lhs.turn < rhs.turn;
    }
    return false;
  }
}
inline bool operator>(const QueKey &lhs, const QueKey &rhs) {
  if (lhs.trainID > rhs.trainID) {
    return true;
  } else {
    if (lhs.trainID == rhs.trainID) {
      return lhs.turn > rhs.turn;
    }
    return false;
  }
}
inline bool operator==(const QueKey &lhs, const QueKey &rhs) {
  return (lhs.trainID == rhs.trainID) && (lhs.turn == rhs.turn);
}
inline bool operator!=(const QueKey &lhs, const QueKey &rhs) {
  return (lhs.trainID != rhs.trainID) || (lhs.turn != rhs.turn);
}

inline bool operator<(const QueVal &lhs, const QueVal &rhs) {
  return lhs.seq < rhs.seq;
}
inline bool operator>(const QueVal &lhs, const QueVal &rhs) {
  return lhs.seq > rhs.seq;
}
inline bool operator==(const QueVal &lhs, const QueVal &rhs) {
  return lhs.seq == rhs.seq;
}
inline bool operator!=(const QueVal &lhs, const QueVal &rhs) {
  return lhs.seq != rhs.seq;
}
#pragma endregion

// Order
#pragma region
inline bool operator<(const OrderVal &lhs, const OrderVal &rhs) {
  return lhs.seq < rhs.seq;
}
inline bool operator>(const OrderVal &lhs, const OrderVal &rhs) {
  return lhs.seq > rhs.seq;
}
inline bool operator==(const OrderVal &lhs, const OrderVal &rhs) {
  return lhs.seq == rhs.seq;
}
inline bool operator!=(const OrderVal &lhs, const OrderVal &rhs) {
  return lhs.seq != rhs.seq;
}
#pragma endregion

#endif