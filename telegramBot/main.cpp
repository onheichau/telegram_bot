#include <iostream>
#include "BotIO.h"
#include "Financewatcher.h"

using namespace std;

int main() {
  Financewatcher watchDog("watch.csv");

  BotIO* godPointer = &watchDog;

  godPointer->run();

  return 0;
}