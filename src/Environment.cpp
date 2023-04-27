#include <iostream>
#include <fstream>
#include <chrono>
#include "Environment.h"
#include "Logger.h"
#include "Timer.h"
#include "Utils.h"

using namespace std;
using namespace utils;

// ============================ private method  ================================

size_t Environment::writeResponseToLog_cb(char* chunkBytes, size_t size, size_t numOfMemBlock, ofstream* log) {
  if(!log->is_open()) {
    log->open(logFile, ios::app);
  }
  *log << static_cast<string>(chunkBytes);
  return size * numOfMemBlock;
}

size_t Environment::writeResponseToBuffer_cb(char* chunkBytes, size_t size, size_t numOfMemBlock, string* responseBuff) {
  *responseBuff += static_cast<string>(chunkBytes);
  return size * numOfMemBlock;
}

Environment& Environment::errorCheck() {
  if(m_result != CURLE_OK) {
    cerr << "libcurl: error code (" << m_result << ") ";
    if(m_errBuffer[0]) {
      Logger::writeToLog(m_errBuffer) << endl;
    } else {
      Logger::writeToLog(curl_easy_strerror(m_result)) << endl;
    }
  }
  return *this;
}

// ============================ public method   ================================
Environment::Environment() {
  // file that log the received resource
/*   m_log.open(logFile, ios::app); */

  // curl initialization
  curl_global_init(CURL_GLOBAL_ALL);
  m_curl = curl_easy_init();

  if(!m_curl) {
    throw("fail to initialize!");
  }

  // set the IP resolution to IPv4 to prevent mac using ipv6
  curl_easy_setopt(m_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

  // set protocol explicitly to have saver operation
  curl_easy_setopt(m_curl, CURLOPT_DEFAULT_PROTOCOL, "https");

  // set the default header for request
  m_header = curl_slist_append(m_header, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:88.0) Gecko/20100101 Firefox/88.0");
  curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_header);

  // set detailed error message destination
  curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, m_errBuffer);

  // config libcurl to "tell more"
  curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 0);

}

Environment::~Environment() {
  curl_slist_free_all(m_header);
  curl_easy_cleanup(m_curl);
  curl_global_cleanup();
}

string Environment::escapeString(const string& str) {
  string escaped;
  char* escapedCstr{};
  escapedCstr = curl_easy_escape(m_curl, str.c_str(), str.length());
  escaped = static_cast<string>(escapedCstr);
  curl_free(escapedCstr);
  return escaped;
}

const string& Environment::responseBuffer() const {
  return m_responseBuf;
}

Environment& Environment::writeToResponseBuffer(const string& data) {
  m_responseBuf.clear();
  m_responseBuf = data;
  return *this;
}

Environment& Environment::setOutputToLog() {
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeResponseToLog_cb);
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, Logger::logAddress());
  return *this;
}

Environment& Environment::setOutputToBuffer() {
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeResponseToBuffer_cb);
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_responseBuf);
  return *this;
}

Environment& Environment::clearResponseBuffer() {
  m_responseBuf.clear();
  return *this;
}

Environment& Environment::createRequest(const string& url) {
  writeToLog("The url is: ") << url.c_str() << endl;
  curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
  Logger::writeToLog(timer.timeStamp()) << "created request => " << url << endl;
  return *this;
}

ofstream& Environment::writeToLog(const string& message) {
  return Logger::writeToLog(timer.timeStamp());
}

Environment& Environment::execute() {
  clearResponseBuffer();
  m_result = curl_easy_perform(m_curl);
  Logger::writeToLog(timer.timeStamp()) << "bytes in response buffer: " << m_responseBuf.length() << endl << endl;
  errorCheck();
  return *this;
}

ostream& Environment::operator<<(const string& logMessage) {
  return writeToLog(logMessage);
}