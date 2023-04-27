#ifndef LOGGER_H__
#define LOGGER_H__
#include <string>
#include <fstream>

class Logger {
public:
  Logger() = default;
  Logger(const Logger& source) = delete;
  Logger& operator=(const Logger& source) = delete;
  ~Logger() = delete;
  static std::ofstream& writeToLog(const std::string& logMsg);
  static void openLogFile(const std::string& fileName);
  static std::ofstream* logAddress();
};

#endif