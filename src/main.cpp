#include <iostream>
#include "Timer.h"

using namespace std;

int main(int argc, char* argv[]) {
  if(argc != 3) {
    cerr << "Invalid argument! do ./executable 'token' 'chatId' <Enter>" << endl;;
  } else {
    // initialize the Financewatcher
    Financewatcher watcher("watch.csv", argv[1], argv[2]);

    timer.loadEvents("event.csv", watcher);

    timer.sortEventList().moveToClosestEvent();

    cout << "sorted event list >===================" << endl;

    for (size_t i = 0; i < timer.m_eventListSize; i++) {
      cout << timer.m_eventsList[i] << endl;
    }

    cout << endl;
    
    cout << "event from the closest ===========================" << endl;

    for (size_t i = timer.m_currentPosition; i < timer.m_eventListSize; i++) {
      cout << "the up coming event is: " << timer.m_eventsList[i] << endl;
    }
  }
  return 0;
}