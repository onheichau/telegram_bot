#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include "Fincialentity.h"
#include "Utils.h"

using namespace std;
using namespace utils;

bool Fincialentity::needNotification() {
  bool needNotification{};
  if(abs(m_currentChangePercentage) >= alertPercentage)  {
      needNotification = true;
  } 
  
  if(abs(m_currentChangePercentage * 0.01) * m_holdingPosition * m_previousClose > 2500) {
      needNotification = true;
  }
  return needNotification;
}

string Fincialentity::createReport() {
  string report;
  ofstream O("report.txt");

  U << O;

  O << "`";
  O << "[Alert] => ";
  O << m_alias;
  O << endl;

  U.print("current price", 20, true);
  U.print(":", 2, true);
  U.print(m_currentPrice, 2, 10, true);
  O << endl;

  U.print("change percentage", 20, true);
  U.print(":", 2, true);
  U.print(m_currentChangePercentage, 2, 10, true);
  O << endl;

  U.print("previous close", 20, true);
  U.print(":", 2, true);
  U.print(m_previousClose, 2, 10, true);
  O << endl;

  U.print("holding position", 20, 1);
  U.print(":", 2, true);
  U.print(m_holdingPosition, 10, true);
  O << endl;

  U.print("change on portfolio", 20, true);
  U.print(":", 2, true);
  U.print(m_holdingPosition * m_previousClose * m_currentChangePercentage * 0.01 , 2, 10, true);
  O << "`";
  O << endl;

  O.close();
  ifstream I("report.txt");
  U << I;

  while (I.good()) {
    char* buff{};
    buff = U.getString(EOF);
    report += static_cast<string>(buff);
    delete[] buff;
  }


/* 
  report += "[Alert] => ";
  report += m_alias;
  report += "\n";
  
  report += "current price      : ";
  report += to_string(m_currentPrice);
  report += "\n";

  report += "change percentage  : ";
  report += to_string(m_currentChangePercentage);
  report += "\n";

  report += "previous close     : ";
  report += to_string(m_previousClose);
  report += "\n";

  report += "holding position   : ";
  report += to_string(m_holdingPosition);
  report += "\n";

  report += "change on portfolio: ";
  report += to_string(m_holdingPosition * m_previousClose * m_currentChangePercentage * 0.01);
  report += "\n"; */

  return report;
}