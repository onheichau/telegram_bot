#include <iostream>
#include <thread>
#include "Utils.h"
#include "MarketMonitor.h"
#include "Timer.h"
#include "UI.h"

using namespace std;
using namespace utils;

int main(int argc, char* argv[]) {
  if(argc != 3) {
    cerr << "Invalid argument! do ./executable 'token' 'chatId' <Enter>" << endl;;
  } else {
    bool ok{};
    int userSelection{-1};

    MarketMonitor monitor("watch.csv", argv[1], argv[2]);
    timer.registerEvents("singleEvent.csv", monitor.callbackMap());
    ok = monitor.botInitTest();
    ok = true;

    if(ok) {
      while (userSelection) {
        userSelection = UI::menu();

        switch (userSelection) {
        case 1:
          UI::eventManagerMenu(monitor);
          break;
        case 2:
          UI::ChatGptManagerMenu();
          break;
        default:
          cout << "Goodbye!\n";
        }
      }
    } else {
      cerr << "fail to pass bot initialization test! check the arguments!\n";
    }
  }
  return 0;
}