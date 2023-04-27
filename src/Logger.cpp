#include <iostream>
#include <fstream>
#include "Logger.h"

using namespace std;

ofstream m_log;

ofstream* Logger::logAddress() {
  return &m_log;
}

ofstream& Logger::writeToLog(const string& logMsg) {
  m_log << logMsg;
  return m_log;
}

void Logger::openLogFile(const std::string& fileName) {
  m_log.open(fileName, ios::app);
  if(!m_log) {
    cerr << "fail to initialize log service!" << "check " << fileName << "\n";
  }
}