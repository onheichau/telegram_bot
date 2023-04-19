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

hh_mm_ss<seconds> Timer::currentHMS() {
  updateToNow();
  return hh_mm_ss<seconds>(hours(floor<hours>(m_now.time_since_epoch()).count() % 24) +
                           minutes(floor<minutes>(m_now.time_since_epoch()).count() % 60) +
                           seconds(floor<seconds>(m_now.time_since_epoch()).count() % 60));
}

unsigned int Timer::currentHour() {
  return (currentHMS().hours().count());
}

unsigned int Timer::currentMinute() {
  return (currentHMS().minutes().count());
}

unsigned int Timer::currentSecond() {
  return (currentHMS().seconds().count());
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

Timer& Timer::enterWorkingStage() {
  while (timeLeftToNextEvent(m_eventsList[m_currentPosition]) >= 0ms) {
    // do nothing , ready for executing event
  }
  
  // log start

  m_eventsList[m_currentPosition].m_callback();

  // log end

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
          currentTimeOfDay = 0ms;
        }
        if(m_eventsList[j].m_timeOfDay >= currentTimeOfDay) {
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
  microseconds wakeupBufferTime = 1500ms;

  microseconds waitTime = timeLeftToNextEvent(m_eventsList[m_currentPosition]);
  if(waitTime > 3000ms) {
    this_thread::sleep_for(timeLeftToNextEvent(m_eventsList[m_currentPosition]) - wakeupBufferTime);
  }

  return *this;
}

microseconds Timer::timeLeftToNextEvent(const Event& nextEvent) {
  updateToNow();
  microseconds offSet = 0ms;
  weekday currentWd{currentWeekDay()};
  microseconds currentTimeOfDay{hours(currentHour()) + minutes(currentMinute()) + seconds(currentSecond())};

  while (currentWd != nextEvent.m_weekday) {
    offSet += microseconds(days(1)) - currentTimeOfDay;
    currentTimeOfDay = 0ms;
    currentWd++;
  }
  offSet += nextEvent.m_timeOfDay - currentTimeOfDay;
  return offSet;
}

Timer& Timer::loadEvents(const char* fileName, Financewatcher& watcher) {
  ifstream input(fileName);
  Event event;
  char* inputBuffer{};
  int wd, hour, minute;
  m_eventListSize = U.grepDashC(regex(","), input);

  input.clear();
  input.seekg(0);

  m_eventsList = new Event[m_eventListSize];

  U << input;

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
    (input >> wd).ignore();
    event.m_weekday = weekday(wd);

    // input day of time from stream
    (input >> hour).ignore();
    (input >> minute).ignore();
    seconds totalSec{hours(hour) + minutes(minute)};
    event.m_timeOfDay = totalSec;

    m_eventsList[i] = event;
  }
  return *this;
}

Timer& Timer::startRoutine() {
  // perhaps it can be some atomic / global control by main thread / UI thread
  bool keepWorking{true}; 
  sortEventList().moveToClosestEvent();
  while (keepWorking) {
    enterWorkingStage();
  }
  return *this;
}

ostream& operator<<(ostream& os, Timer& rhs) {
  os << rhs.timeStamp();
  return os;
}

int main() {
  Financewatcher watcher("watch.csv");
  timer.loadEvents("event.csv", watcher);
  timer.sortEventList();
  
  for (size_t i = 0; i < timer.m_eventListSize; i++) {
/*     cout << "event weekday: " << timer.m_eventsList[i].m_weekday.c_encoding() << " "
    << "event Hour: " << duration<float, ratio<3600>>(timer.m_eventsList[i].m_timeOfDay).count() << " " */
    cout << "time left to this event: " << duration<float, ratio<3600>>(timer.timeLeftToNextEvent(timer.m_eventsList[i])).count()
    << endl;
  }

  return 0;
}