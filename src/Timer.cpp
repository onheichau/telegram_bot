#include <iostream>
#include <fstream>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include "Logger.h"
#include "Timer.h"
#include "Utils.h"

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

Timer::~Timer() {
  delete[] m_eventsList;
}

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

Timer& Timer::enterWorkingStage() {
  int count{};

  // busy waiting , ready for executing event
  while (timeLeftToNextEvent(m_eventsList[m_currentPosition]) >= 0ms && timeLeftToNextEvent(m_eventsList[m_currentPosition]) < 1s) {
    count++;
  }

  if(count) {
    Logger::writeToLog(timeStamp()) << "found " << count << "cpu cycle while busy waiting\n" 
    << "m_current position: " << m_currentPosition << "\n"
    << m_eventsList[m_currentPosition] << endl;
  }

  Logger::writeToLog(timeStamp()) << "========================> Start" << endl;

  (*(m_eventsList[m_currentPosition].m_callback))();

  Logger::writeToLog(timeStamp()) << "========================> End\n" << endl;

  if(m_currentPosition < m_eventListSize - 1) {
    m_currentPosition++;
  } else {
    Logger::writeToLog(timeStamp()) << "Event list rewinds\n";
    m_currentPosition = 0; // start over from sunday
  }
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

        if(m_eventsList[j].m_weekday != currentWd) {
          currentWd++;
          currentTimeOfDay = 0ms;
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
  seconds buffer = 1s;
  seconds waitTime = duration_cast<seconds>(timeLeftToNextEvent(m_eventsList[m_currentPosition]));
  unique_lock<mutex> lock(m_mutex);

  if(waitTime > 100ms) {
    Logger::writeToLog(timeStamp()) << "Sleep for " << seconds(duration_cast<seconds>(waitTime)).count()
                                    << " seconds " << "aka " << duration<double, ratio<86400>>(waitTime).count() << "days\n";
                                    Logger::logAddress()->flush();
    m_cv.wait_for(lock, waitTime + buffer, [this]() {return !m_keepRunning;});
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

  // same day but already passed
  if(offSet < 0ms && currentWd == nextEvent.m_weekday) {
    offSet += days(7);
  }

  return offSet;
}

/// @brief register events to the timer internal Event list
/// @param fileName a comma delimited csv file to read. format : callbackIdentifier,weekday,hour,minute
/// @param callbackMap this unordered_map store the pairs of callback identifier and the relative callback instance
/// @return reference of caller
Timer& Timer::registerEvents(const string& fileName, unordered_map<string, function<void()>>& callbackMap) {
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
    event.m_callback = addressof(callbackMap[string(inputBuffer)]);
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
  sortEventList();
  return *this;
}

Timer& Timer::startRoutine() {
  m_keepRunning = true;

  sortEventList().moveToClosestEvent();

  while (m_keepRunning) {
    waitForNextEvent();
    if(m_keepRunning) {
      enterWorkingStage();
    }
  }
  return *this;
}

Timer& Timer::saveEvents(const string& fileName, unordered_map<string, function<void()>>& callbackMap) {
  bool matched{}, allGood{true};
  string buffer;

  for (size_t i = 0; i < m_eventListSize && allGood; i++) {
    hh_mm_ss<seconds> hms{m_eventsList[i].m_timeOfDay};
    matched = false;

    // write identifier
    for(unordered_map<string, function<void()>>::iterator it = callbackMap.begin(); it != callbackMap.end() && !matched; it++) {
      if(m_eventsList[i].m_callback == addressof(it->second)) {
        matched = true;
        buffer += it->first ;
        buffer += ",";
      }
    }

    if(!matched) {
      allGood = false;
      cerr << "error to match back to identifier!" << endl;
    } else {
      // write week day 
      buffer += to_string(m_eventsList[i].m_weekday.c_encoding()) ;
      buffer += ",";

      // write time back
      buffer += to_string(hms.hours().count());
      buffer += ":"; 
      buffer += to_string(hms.minutes().count()); 
      buffer += "\n";
    }
  }

  if(allGood) {
    ofstream out(fileName);

    out? (out << buffer) : (cerr << "file is not in good state! check " << fileName << endl);
  }
  return *this;
}

ostream& operator<<(ostream& os, Timer& rhs) {
  os << rhs.timeStamp();
  return os;
}

ostream& Timer::listNext10Events(ostream& os) {
  moveToClosestEvent();

  U << os;
  U.print("\n", 70, true, '+') << "\n";
  os << "The next 10 events are:\n";

  int count = 0;
  size_t i = m_currentPosition;

  while (i < m_eventListSize && count < 10) {
    os << count + 1 << " - " << m_eventsList[i++] << "\n";

    // beyond the boundary of array
    if(i > m_eventListSize - 1) i = 0;

    count++;
  }
  
  for (size_t i = m_currentPosition; i < m_eventListSize && count < 10; i++) {
  }
  U.print("+", 70, true, '+') << "\n\n";
  return os;
}

Timer& Timer::stopRoutine() {
/*   cout << "received stopping signal\n"; */
  m_keepRunning = false;
  m_cv.notify_all();
  return *this;
}

bool Timer::isRunning() const {
  return m_keepRunning;
}