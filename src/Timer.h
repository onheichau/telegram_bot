#ifndef TIMER_H_
#define TIMER_H_
#include <iostream>
#include <chrono>
#include <string>
#include "Event.h"

class Timer {
public:
  std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> m_now;
  std::chrono::year_month_day m_ymd;
  Event* m_eventsList{};
  size_t m_eventListSize{};
  size_t m_currentPosition{};

  Timer& updateToNow();
  int currentYear();
  unsigned int currentMonth();
  unsigned int currentDay();
  unsigned int currentHour();
  unsigned int currentMinute();
  unsigned int currentSecond();
  unsigned int currentWeekDay();
  std::chrono::hh_mm_ss<std::chrono::seconds> currentHMS();
  std::string getWeekDayName(const unsigned int weekday);
  Timer& sortEventList();
  void clearEventList();
  Timer& enterWorkingStage();
  Timer& moveToClosestEvent();
  Timer& waitForNextEvent();

/*   Timer& registerEvent(); // implement for dynamically add event later */


  Timer();
  Timer(const Timer& source) = delete;
  Timer& operator=(const Timer& source) = delete;
  ~Timer() = default;
  std::string timeStamp();
  std::chrono::microseconds timeLeftToNextEvent(const Event& nextEvent);
  Timer& loadEvents(const char* fileName, Financewatcher& watcher);
  Timer& startRoutine();
};

// write the time stamp to ostream
std::ostream& operator<<(std::ostream& os, Timer& rhs);

extern Timer timer;

#endif

