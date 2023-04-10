#include <iostream>
#include <fstream>
#include "Environment.h"
#include "Utils.h"

using namespace std;
using namespace utils;

// ============================ private method  ================================

// return a string that contain api, token and chatid in url format
string Environment::read(const string& fileName) {
  char WARN_NULLPTR_RETURNED[]{"Warning: no character is read! nullptr is returned!"};
  ifstream input(fileName);
  char* data{};
  U << input;

  if(input.is_open()) {
    // validate JSON format
    (input.peek() == '{' && input.ignore(20, ':')) || (cout << "invalid JSON file!");
    input.ignore(2);

Environment& Environment::errorCheck() {
  if(m_result != CURLE_OK) {
    cerr << "libcurl: error code (" << m_result << ") ";
    if(m_errBuffer[0]) {
      m_log << m_errBuffer << endl;
    } else {
      m_log << curl_easy_strerror(m_result) << endl;
    }
  }
  return *this;
}

// ============================ public method   ================================
Environment::Environment() {
  // file that log the received resource
  m_log.open(logFile, ios::app);

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
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_log);
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
  curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
  m_log << timeStamp() << "created request => " << url << endl;
  return *this;
}


string Environment::timeStamp() {
  auto currentTime = chrono::system_clock::now();
  time_t time = chrono::system_clock::to_time_t(currentTime);
  string timeStamp;
  timeStamp += "[";
  timeStamp += ctime(&time);
  timeStamp[timeStamp.length() - 1] = ']';
  timeStamp += " ";
  return timeStamp;
}

ofstream& Environment::writeToLog(const string& message) {
  m_log << message;
  return m_log;
}

Environment& Environment::execute() {
  clearResponseBuffer();
  m_result = curl_easy_perform(m_curl);
  m_log << timeStamp() << "bytes in response buffer: " << m_responseBuf.length() << endl << endl;
  errorCheck();
  return *this;
}
