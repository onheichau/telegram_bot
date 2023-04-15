#ifndef TIME_H_
#define TIME_H_
#include <iostream>
#include <chrono>
#include <string>

const unsigned int TIME_SUNDAY = 0;
const unsigned int TIME_MONDAY = 1;
const unsigned int TIME_TUESDAY = 2;
const unsigned int TIME_WEDNESDAY = 3;
const unsigned int TIME_THURSDAY = 4;
const unsigned int TIME_FRIDAY = 5;
const unsigned int TIME_SATURDAY = 6;

class Time {
  std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> m_now;
  std::chrono::year_month_day m_ymd;
  Time& updateToNow();

public:
  Time();
  Time(const Time& source) = delete;
  Time& operator=(const Time& source) = delete;
  ~Time() = default;
  int currentYear();
  unsigned int currentMonth();
  unsigned int currentDay();
  unsigned int currentWeekDay();
  unsigned int currentHour();
  unsigned int currentMinute();
  unsigned int currentSecond();
  std::string timeStamp();
};

#endif

