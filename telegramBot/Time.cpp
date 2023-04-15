#include <iostream>
#include <chrono>
#include <string>
#include "Time.h"

using namespace std;
using namespace chrono;
using namespace chrono_literals;


Time& Time::updateToNow() {
  const hours EDT_UTC_TIME_DIFF = -4h;
  m_now = time_point_cast<seconds>(system_clock::now() + EDT_UTC_TIME_DIFF);
  m_ymd = floor<days>(m_now);
  return *this;
}

Time::Time() {
  updateToNow();
}

int Time::currentYear() {
  updateToNow();
  return (int)m_ymd.year();
}

unsigned int Time::currentMonth() {
  updateToNow();
  return (unsigned int)m_ymd.month();
}

unsigned int Time::currentDay() {
  updateToNow();
  return (unsigned int)m_ymd.day();
}

unsigned int Time::currentWeekDay() {
  updateToNow();
  weekday wd;
  return wd.__weekday_from_days(currentDay());
}

unsigned int Time::currentHour() {
  updateToNow();
  return (m_now.time_since_epoch().count() / 3600) % 24;
}

unsigned int Time::currentMinute() {
  return (m_now.time_since_epoch().count() / 60) % 60;
}

unsigned int Time::currentSecond() {
  return m_now.time_since_epoch().count() % 60;
}

string Time::timeStamp() {
  string timeStamp;
  timeStamp += "[Date: ";
  timeStamp += (to_string(currentYear()) + "-");
  timeStamp += (to_string(currentMonth()) + "-");
  timeStamp += (to_string(currentDay()));
  timeStamp += " | Time: ";
  timeStamp += (to_string(currentHour()) + ":");
  timeStamp += (to_string(currentMinute()) + ":");
  timeStamp += (to_string(currentSecond()));
  timeStamp += "] ";
  return timeStamp;
}

int main() {
  Time T;
  cout << T.timeStamp() << endl;
  return 0;
}