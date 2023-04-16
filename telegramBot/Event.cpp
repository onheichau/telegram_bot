#include <iostream>
#include <chrono>
#include "Event.h"

using namespace std;
using namespace chrono;


Event::Event(function<void()> callback, const int weekday, const chrono::seconds timeOfDay) {

}

bool Event::operator>(const Event& rhs) {
  bool thisEventIsLater{};
  if(m_weekday.c_encoding() > rhs.m_weekday.c_encoding()) {
    thisEventIsLater = true;
  } else if ((m_weekday == rhs.m_weekday) && (m_timeOfDay > rhs.m_timeOfDay)) {
    thisEventIsLater = true;
  }
  return ;
}

bool Event::operator<(const Event& rhs) {
  return (!(*this > rhs) && !(*this == rhs));
}

bool Event::operator==(const Event& rhs) {
  return ((m_weekday == rhs.m_weekday) && (m_timeOfDay == rhs.m_timeOfDay));
}
