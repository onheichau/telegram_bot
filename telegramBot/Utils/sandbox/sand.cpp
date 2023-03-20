#include <iostream>
#include <regex>

using namespace std;

int main() {
  char userInput[20]{"00010009.0000"};
  regex positiveInt("^[0-9]+([.][0]+)?$");
  regex validDigit("(^[0]*)([0-9]+).*$");

  if(regex_match(userInput, positiveInt)) {
    cout << "valid input" << endl;

    cout << "the string is: " << userInput << endl;

    cout << "replaced version is: " << regex_replace(userInput, validDigit, "$2") << endl;

  } else {
    cout << "invalid input" << endl;
  }

  return 0;
}