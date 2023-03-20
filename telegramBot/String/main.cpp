// Name: On Hei Chau
// Email: ochau2@myseneca.ca
// Student Id: 172917213

// File: main.cpp
// Beyond 4096 challenge 
// Date of Completion: 2023-03-05 (YYYY-MM-DD)

// I have done all the coding by myself and only copied the code that my professor
// provided to complete my workshops and assignments.
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "String.h"

using namespace std;
using namespace sdds;

int main() {
  String S{"Hello!"};
  ifstream file{"story.txt"};
  ofstream output("output.txt");
  

  cout << S << endl;

  cout << "put some text: ";
  cin >> S;

  cout << S << endl;

  S = "";
  file >> S;
  cout << S << endl;

  S = "";
  S.setImportWholeFile();
  file >> S;
  cout << S;
  output << S;

  return 0;
}