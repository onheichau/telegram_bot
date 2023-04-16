#ifndef TIMER_H_
#define TIMER_H_
#include <iostream>
#include <chrono>
#include <string>
#include "Event.h"

class Timer {
  std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> m_now;
  std::chrono::year_month_day m_ymd;
  Event* m_eventsList{};
  size_t m_eventListSize;

  Timer& updateToNow();
  int currentYear();
  unsigned int currentMonth();
  unsigned int currentDay();
  unsigned int currentHour();
  unsigned int currentMinute();
  unsigned int currentSecond();
  unsigned int currentWeekDay();
  std::string getWeekDayName(const unsigned int weekday);
  Timer& sortEventList();
  Timer& resetEventList();
  Timer& executeCallback(std::function<void()>);
  Timer& moveToClosestEvent();
  Timer& waitForNextEvent();
  std::chrono::seconds secondsLeftToNextEvent();
  Timer& loadEventList(const char* fileName);

/*   Timer& registerEvent(); // implement for dynamically add event later */


public:
  Timer();
  Timer(const Timer& source) = delete;
  Timer& operator=(const Timer& source) = delete;
  ~Timer() = default;
  std::string timeStamp();
  Timer& startRoutine();
};

extern Timer timer;

#endif

