#ifndef BOTIO_H__
#define BOTIO_H__
#include <iostream>
#include <fstream>
#include <string>


class BotIO {
protected:

  // returns a string of NOW(time)
  std::string timeStamp();

  /// @brief append the log file
  /// @param message message to log
  /// @return reference of log file, client can continue logging using << operator
  std::ofstream& writeToLog(const std::string& message);

  // returns the read-only buffer string that store http response
  const std::string responseBuffer();

  // returned html will store in responseBuffer() as a string
  const std::string& fetchHtml(const std::string& url);

  BotIO& sendMessageToTelegram(const std::string& msg);

public:
  bool botInitTest();
  BotIO(const std::string& token, const std::string& chatId);
  virtual ~BotIO() = default;
};

#endif