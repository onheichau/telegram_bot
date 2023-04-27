#ifndef TIMER_H_
#define TIMER_H_
#include <iostream>
#include <chrono>
#include <string>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "Event.h"

class Timer {
  std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> m_now;
  std::chrono::year_month_day m_ymd;
  Event* m_eventsList{};
  size_t m_eventListSize{};
  size_t m_currentPosition{};
  std::atomic<bool> m_keepRunning{};
  std::condition_variable m_cv{};
  std::mutex m_mutex{};

  Timer& updateToNow();
  int currentYear();
  unsigned int currentMonth();
  unsigned int currentDay();
  unsigned int currentHour();
  unsigned int currentMinute();
  unsigned int currentSecond();
  unsigned int currentWeekDay();
  std::chrono::hh_mm_ss<std::chrono::seconds> currentHMS();
  Timer& sortEventList();
  Timer& enterWorkingStage();
  Timer& moveToClosestEvent();
  Timer& waitForNextEvent();
  std::chrono::microseconds timeLeftToNextEvent(const Event& nextEvent);

public:
  Timer() = default;
  Timer(const Timer& source) = delete;
  Timer& operator=(const Timer& source) = delete;
  ~Timer();
  std::string timeStamp();
  std::string getWeekDayName(const unsigned int weekday);
  Timer& registerEvents(const std::string& fileName, std::unordered_map<std::string, std::function<void()>>& callbackMap);
  Timer& startRoutine();
  Timer& stopRoutine();
  Timer& saveEvents(const std::string& fileName, std::unordered_map<std::string, std::function<void()>>& callbackMap);
  std::ostream& listNext10Events(std::ostream& os = std::cout);
  bool isRunning() const;
};

// write the time stamp to ostream
std::ostream& operator<<(std::ostream& os, Timer& rhs);

extern Timer timer;

#endif

