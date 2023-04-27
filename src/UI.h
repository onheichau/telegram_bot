#ifndef UI_H__
#define UI_H__
#include <iostream>

class UI {
public:
  static int menu();
  static void eventManagerMenu(BotIO& bot);
  static int ChatGptManagerMenu();
  UI() = default;
  ~UI() = default;
};





#endif