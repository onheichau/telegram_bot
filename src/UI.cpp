#include <iostream>
#include <thread>
#include "Logger.h"
#include "MarketMonitor.h"
#include "BotIO.h"
#include "Timer.h"
#include "Utils.h"
#include "UI.h"

using namespace std;
using namespace utils;

thread scheduler{};

int UI::menu() {
  U.print("\n", 70, true, '=') << "\n";
  cout << "Main Menu\n";
  U.print("=", 70, true, '=') << "\n\n";
  cout << "1 - Event Manager\n"
       << "2 - ChatGPT Manager\n"
       << "3 - Whatever\n"
       << "4 - Whatever\n"
       << "0 - Exit\n"
       << "> ";
  return U.getNumber(0, 2, "invalid integer!\n> ");
}

void UI::eventManagerMenu(BotIO& bot) {
  int userSelection{-1};

  while (userSelection) {
    U.print("\n", 70, true, '=') << "\n";
    cout << "Event Manager\n";
    U.print("=", 70, true, '=') << "\n\n";
    cout << "1 - List next 10 events\n"
         << "2 - Start\n"
         << "3 - Stop\n"
         << "4 - Add Event\n"
         << "5 - Remove Event\n"
         << "0 - Back to main menu\n"
         << "> ";
    userSelection = U.getNumber(0, 3, "invalid integer\n> ");
    switch (userSelection) {
    case 1:
      timer.listNext10Events();
      break;
    case 2:
      U.print("\n", 70, true, '+') << "\n";
      if(!timer.isRunning()) {
        U << cout;
        cout << "scheduler is now operating.\n";
        Logger::writeToLog(timer.timeStamp()) << "scheduler thread start\n";
        scheduler = thread(bind(&Timer::startRoutine, &timer));
      } else {
        cout << "scheduler is already operating.\n";
      }
      U.print("+", 70, true, '+') << "\n\n";
      break;
    case 3:
      U.print("\n", 70, true, '+') << "\n";
      if(!timer.isRunning()) {
        cout << "scheduler is not operating.\n";
      } else {
        timer.stopRoutine(); // stop the thread
        scheduler.join();
        timer.saveEvents("save.csv", dynamic_cast<MarketMonitor&>(bot).callbackMap());
        cout << "scheduler is terminated safely.\n";
        Logger::writeToLog(timer.timeStamp()) << "scheduler thread terminated safely\n";
      }
      U.print("+", 70, true, '+') << "\n\n";
      break;
    default:
      cout << "";
    }
  }
}

int UI::ChatGptManagerMenu() {
  int userSelection{};
  cout << "To be implemented\n";
  return userSelection;
}

void UI::cleanUp(BotIO& bot) {
  if(timer.isRunning()) {
    timer.stopRoutine(); // stop the thread
    scheduler.join();
    timer.saveEvents("save.csv", dynamic_cast<MarketMonitor &>(bot).callbackMap());
    cout << "scheduler is terminated safely.\n";
    Logger::writeToLog(timer.timeStamp()) << "scheduler thread terminated safely\n";
  }
}
