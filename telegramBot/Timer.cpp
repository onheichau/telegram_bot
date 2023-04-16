#include <iostream>
#include <chrono>
#include <string>
#include "Timer.h"

using namespace std;
using namespace chrono;
using namespace chrono_literals;

Timer timer;

Timer& Timer::updateToNow() {
  const hours EDT_UTC_TIME_DIFF = -4h;
  m_now = time_point_cast<seconds>(system_clock::now() + EDT_UTC_TIME_DIFF);
  m_ymd = floor<days>(m_now);
  return *this;
}

Timer::Timer() { }

int Timer::currentYear() {
  updateToNow();
  return (int)m_ymd.year();
}

unsigned int Timer::currentMonth() {
  updateToNow();
  return (unsigned int)m_ymd.month();
}

unsigned int Timer::currentDay() {
  updateToNow();
  return (unsigned int)m_ymd.day();
}

unsigned int Timer::currentWeekDay() {
  updateToNow();
  weekday wd;
  return wd.__weekday_from_days(m_ymd.__to_days().count());
}

unsigned int Timer::currentHour() {
  updateToNow();
  return (m_now.time_since_epoch().count() / 3600) % 24;
}

unsigned int Timer::currentMinute() {
  return (m_now.time_since_epoch().count() / 60) % 60;
}

unsigned int Timer::currentSecond() {
  return m_now.time_since_epoch().count() % 60;
}

std::string Timer::getWeekDayName(const unsigned int weekday) {
  string name;
  switch (weekday) {
  case 0:
    name = "Sun";
    break;
  case 1:
    name = "Mon";
    break;
  case 2:
    name = "Tue";
    break;
  case 3:
    name = "Wed";
    break;
  case 4:
    name = "Thr";
    break;
  case 5:
    name = "Fri";
    break;
  case 6:
    name = "Sat";
    break;
  default:
    cerr << "invalid week day detected!\n";
    break;
  }
  return name;
}
string Timer::timeStamp() {
  string timeStamp;
  timeStamp += "[Date: ";
  timeStamp += (to_string(currentYear()) + "-");

  if(currentMonth() < 10) {
    timeStamp += "0";
  }
  timeStamp += (to_string(currentMonth()) + "-");
  if(currentDay() < 10) {
    timeStamp += "0";
  }
  timeStamp += (to_string(currentDay()));

  timeStamp += " ";
  timeStamp += getWeekDayName(currentWeekDay());

  timeStamp += " | Time: ";

  if(currentHour() < 10) {
    timeStamp += "0";
  }
  timeStamp += (to_string(currentHour()) + ":");
  if(currentMinute() < 10) {
    timeStamp += "0";
  }
  timeStamp += (to_string(currentMinute()) + ":");
  if(currentSecond() < 10) {
    timeStamp += "0";
  }
  timeStamp += (to_string(currentSecond()));
  timeStamp += "] ";
  return timeStamp;
}
