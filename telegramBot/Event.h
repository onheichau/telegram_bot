#ifndef EVENT_H__
#define EVENT_H__
#include <iostream>
#include <chrono>
#include "Financewatcher.h"

struct Event {
  std::function<void()> m_callback;; // call back function to execute
  std::chrono::weekday m_weekday;
  std::chrono::seconds m_timeOfDay;

  Event() = default;
  Event(std::function<void()> callback, const int weekday, const std::chrono::seconds timeOfDay);
  Event(const Event& source) = delete;
  Event& operator=(const Event& source) = delete;

  // return true if event at left hand side trigger day and timer is later then event at rhs
  // Sunday 00:00 is defined as the earliest event trigger day and time
  bool operator>(const Event& rhs);

  bool operator<(const Event& rhs);
  bool operator==(const Event& rhs);
};

#endif