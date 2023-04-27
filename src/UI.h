#ifndef UI_H__
#define UI_H__
#include <iostream>
#include "BotIO.h"

class UI {
public:
  static int menu();
  static void eventManagerMenu(BotIO& bot);
  static int ChatGptManagerMenu();
  static void cleanUp(BotIO& bot);
  UI() = default;
  ~UI() = default;
};





#endif