#include <iostream>
#include <unordered_map>
#include "Timer.h"

using namespace std;

int main(int argc, char* argv[]) {
  if(argc != 3) {
    cerr << "Invalid argument! do ./executable 'token' 'chatId' <Enter>" << endl;;
  } else {
    // initialize the MarketMonitor
    MarketMonitor monitor("watch.csv", argv[1], argv[2]);

    timer.registerEvents("singleEvent.csv", monitor.callbackMap());

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

    function<void()>* ptr = timer.m_eventsList[timer.m_currentPosition].m_callback;
    cout << "the address of the callback stored in event is: " << ptr << endl;

    cout << "extract a pointer from map, then call it: " << endl;
    function<void()>* callablePtr = addressof(monitor.callbackMap()["UsReport"]);
    cout << "address of callable ptr is: " << callablePtr << endl;
    (*callablePtr)();

    cout << "call the function from pointer of call back list" << endl;
    function<void()>* callbackListEventCallbackPtr = timer.m_eventsList[timer.m_currentPosition].m_callback;
    cout << "address of the ptr from event list is: " << callbackListEventCallbackPtr << endl;
    (*callbackListEventCallbackPtr)();

    for(auto it = monitor.callbackMap().begin(); it != monitor.callbackMap().end(); it++) {
      cout << "the address of the UsReport in map is: " << addressof(it->second) << endl;
    }

    timer.saveEvents("save.csv", monitor.callbackMap());
  }
  return 0;
}