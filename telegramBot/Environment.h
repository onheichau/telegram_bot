#ifndef ENVIRONMENT_H__
#define ENVIRONMENT_H__
#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>


class Environment {
  CURL* m_curl{};
  CURLcode m_result{};
  std::string m_chatId{};
  std::string m_apiEndPoint{};
  FILE* m_logPtr{};
  char m_errBuffer[CURL_ERROR_SIZE + 1]{0};


  // return a string that contain api, token and chat id in url format
  std::string read(const std::string& fileName);
public:
  Environment(const std::string& fileName);
  Environment(const Environment& source) = delete;
  Environment& operator=(const Environment& source) = delete;
  ~Environment();
  std::ostream& errorCheck(std::ostream& os = std::cout) const;
  CURLcode addHeaderMsg(const std::string& msg);
  std::string timeStamp();
  Environment& writeToLog(const std::string& logMsg);
  Environment& createRequest(const std::string& method, const std::string& message);
  Environment& execute(std::ostream& log = std::cout);
};

#endif