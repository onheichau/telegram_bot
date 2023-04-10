#ifndef ENVIRONMENT_H__
#define ENVIRONMENT_H__
#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>

const char logFile[] = "log.txt";

class Environment {
  CURL* m_curl{};
  CURLcode m_result{};
  curl_slist* m_header{};
  std::ofstream m_log{};
  std::string m_responseBuf{};
  char m_errBuffer[CURL_ERROR_SIZE + 1]{0};

  // call back function to write the response to log file
  static size_t writeResponseToLog_cb(char* chunkBytes, size_t size, size_t numOfMemBlock, std::ofstream* log);

  // call back function to write the response to the temporary buffer of a string
  static size_t writeResponseToBuffer_cb(char* chunkBytes, size_t size, size_t numOfMemBlock, std::string* responseBuff);

  // check the status of return code of libcurl. If error is catch, it will write the detail to log
  Environment& errorCheck();
public:

  Environment();
  Environment(const Environment& source) = delete;
  Environment& operator=(const Environment& source) = delete;
  ~Environment();

  // encodes illegal characters in an url  of a string
  std::string escapeString(const std::string& str);

  // returns the read-only buffer string that store http response
  const std::string& responseBuffer() const;

  // append the response buffer
  Environment& writeToResponseBuffer(const std::string& data);

  // set the following libcurl output to log file
  Environment& setOutputToLog();
  
  // set the following libcurl output to response buffer
  Environment& setOutputToBuffer();

  Environment& clearResponseBuffer();

  // returns a string of NOW(time)
  std::string timeStamp();

  /// @brief append the log file
  /// @param message message to log
  /// @return reference of log file, client can continue logging using << operator
  std::ofstream& writeToLog(const std::string& message);

  Environment& createRequest(const std::string& url);
  Environment& execute();

  // alias of writeToLog
  std::ostream& operator<<(const std::string& logMessage);
};

#endif