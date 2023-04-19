#ifndef EVENT_H__
#define EVENT_H__
#include <iostream>
#include <chrono>
#include "MarketMonitor.h"

struct Event {
  std::function<void()>* m_callback{}; 
  std::chrono::weekday m_weekday;
  std::chrono::seconds m_timeOfDay;

  Event() = default;

  // return true if event at left hand side trigger day and timer is later then event at rhs
  // Sunday 00:00 is defined as the earliest event trigger day and time
  bool operator>(const Event& rhs);

  bool operator<(const Event& rhs);
  bool operator==(const Event& rhs);
};

std::ostream& operator<<(std::ostream& os, const Event& rhs);

#endif