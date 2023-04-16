#ifndef TIMER_H_
#define TIMER_H_
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

class Timer {
  std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> m_now;
  std::chrono::year_month_day m_ymd;
  Timer& updateToNow();
  int currentYear();
  unsigned int currentMonth();
  unsigned int currentDay();
  unsigned int currentHour();
  unsigned int currentMinute();
  unsigned int currentSecond();
  unsigned int currentWeekDay();
  std::string getWeekDayName(const unsigned int weekday);

public:
  Timer();
  Timer(const Timer& source) = delete;
  Timer& operator=(const Timer& source) = delete;
  ~Timer() = default;
  std::string timeStamp();
};

extern Timer timer;

#endif

