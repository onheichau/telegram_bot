#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <thread>
#include "Timer.h"
#include "Utils.h"
#include "Financewatcher.h"

using namespace std;
using namespace chrono;
using namespace chrono_literals;
using namespace utils;

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

Timer& Timer::sortEventList() {
    if (m_eventListSize == 0) {
        return *this;
    }
    for (size_t i = 0; i < m_eventListSize - 1; i++) {
        for (size_t j = 0; j < m_eventListSize - i - 1; j++) {
            if (m_eventsList[j] > m_eventsList[j + 1]) {
                U.swap(m_eventsList[j], m_eventsList[j + 1]);
            }
        }
    }
    return *this;
}

void Timer::clearEventList() {
  m_currentPosition = 0;
}

Timer& Timer::executeCallback() {
  m_eventsList[m_currentPosition].m_callback();
  if(m_currentPosition < m_eventListSize) {
    m_currentPosition++;
  } else {
    clearEventList();
  }
  waitForNextEvent();
  return *this;
}

Timer& Timer::moveToClosestEvent() {
  updateToNow();
  weekday currentWd = weekday(currentWeekDay());
  microseconds currentTimeOfDay{hours(currentHour()) + minutes(currentMinute()) + seconds(currentSecond())};

  bool found{};

  for (size_t i = 0; i < m_eventListSize && !found; i++) {

    // search from the same week day of now
    if(currentWd == m_eventsList[i].m_weekday) {

      // core loop
      for (size_t j = i; j < m_eventListSize && !found; j++, i++) {
        if(m_eventsList[j + 1].m_weekday != currentWd) {
          currentWd++;
          currentTimeOfDay = 1ms;
        }
        if(m_eventsList[j].m_timeOfDay > currentTimeOfDay) {
          found = true;
          m_currentPosition = j;
        }
      }
    }
  }
  if(!found) {
    m_currentPosition = 0;
  }
  return *this;
}

Timer& Timer::waitForNextEvent() {
  this_thread::sleep_for(secondsLeftToNextEvent() - 1500ms);
  return *this;
}

microseconds Timer::secondsLeftToNextEvent() {
  updateToNow();
  microseconds offSet = 0ms;
  weekday currentWd{currentWeekDay()};
  microseconds currentTimeOfDay{hours(currentHour()) + minutes(currentMinute()) + seconds(currentSecond())};

  while (currentWd != m_eventsList[m_currentPosition].m_weekday) {
    offSet += microseconds(days(1)) - currentTimeOfDay;
    currentTimeOfDay = 0ms;
    currentWd++;
  }
  offSet += m_eventsList[m_currentPosition].m_timeOfDay - currentTimeOfDay;
  return offSet;
}

Timer& Timer::loadEvents(const char* fileName, Financewatcher& watcher) {
  ifstream listInput(fileName);
  Event event;
  char* inputBuffer{};
  int wd, hour, minute, second;
  m_eventListSize = U.grepDashC(regex(","), listInput);

  m_eventsList = new Event[m_eventListSize];

  for (size_t i = 0; i < m_eventListSize; i++) {

    // map call back to event
    inputBuffer = U.getString(',');
    if (U.strcmp(inputBuffer, "UsReport") == 0)
    {
      event.m_callback = [&watcher]() { watcher.sendUsMarketDayReport_cb(); };
    }
    else if (U.strcmp(inputBuffer, "AsiaReport") == 0)
    {
      event.m_callback = [&watcher]() { watcher.sendAsiaMarketDayReport_cb(); };
    }
    else if (U.strcmp(inputBuffer, "UsMonitor") == 0)
    {
      event.m_callback = [&watcher]() { watcher.sendUsMarketDayReport_cb(); };
    }
    else if (U.strcmp(inputBuffer, "AsiaMonitor") == 0)
    {
      event.m_callback = [&watcher]() { watcher.monitorAsiaMarketPrice_cb(); };
    }
    else
    {
      cerr << "invalid event identifier detected!\n";
    }
    delete[] inputBuffer;

    // input week day from stream
    (listInput >> wd).ignore();
    event.m_weekday = weekday(wd);

    // input day of time from stream
    (listInput >> hour).ignore();
    (listInput >> minute).ignore();
    (listInput >> second).ignore();
    seconds totalSec{hours(hour) + minutes(minute) + seconds(second)};
    event.m_timeOfDay = totalSec;
    
  }
  
  return *this;
}
