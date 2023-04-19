#define CATCH_CONFIG_MAIN
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include "catch.hpp"
#include "../Timer.h"
#include "../Utils.h"
#include "../Event.h"
#include "../Financewatcher.h"

using namespace std;
using namespace utils;
using namespace chrono;
using namespace std::chrono_literals;

TEST_CASE("timeLeftToNextEvent") {
  Financewatcher watcher("../watch.csv");
  ifstream input("../event.csv");
  Event *ptr;
  Event event;
  size_t size = U.grepDashC(regex(","), input);
  ptr = new Event[size];

  char *inputBuffer{};
  int wd, hour, minute, second;


  for (size_t i = 0; i < size; i++) {

    // map call back to event
    inputBuffer = U.getString(',');
    if (U.strcmp(inputBuffer, "UsReport") == 0)
    {
      event.m_callback = [&watcher]()
      { watcher.sendUsMarketDayReport_cb(); };
    }
    else if (U.strcmp(inputBuffer, "AsiaReport") == 0)
    {
      event.m_callback = [&watcher]()
      { watcher.sendAsiaMarketDayReport_cb(); };
    }
    else if (U.strcmp(inputBuffer, "UsMonitor") == 0)
    {
      event.m_callback = [&watcher]()
      { watcher.sendUsMarketDayReport_cb(); };
    }
    else if (U.strcmp(inputBuffer, "AsiaMonitor") == 0)
    {
      event.m_callback = [&watcher]()
      { watcher.monitorAsiaMarketPrice_cb(); };
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
  }

  for (size_t i = 0; i < size; i++) {
    cout << duration<float, ratio<3600>>(timer.timeLeftToNextEvent(ptr[i])).count() << endl;
  }
  
}
