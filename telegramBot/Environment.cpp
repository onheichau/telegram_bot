#include <iostream>
#include <fstream>
#include "Environment.h"
#include "Utils.h"

using namespace std;
using namespace sdds;

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

    // fetch api
    data = U.getString('"', WARN_NULLPTR_RETURNED);
    m_apiEndPoint += data;
    input.ignore(20, ':');
    input.ignore(10, '"');
    delete[] data; data = nullptr;

    // fetch token
    data = U.getString('"', WARN_NULLPTR_RETURNED);
    m_apiEndPoint += data;
    input.ignore(20, ':');
    input.ignore(10, '"');
    delete[] data; data = nullptr;

    // fetch charId
    data = U.getString('"', WARN_NULLPTR_RETURNED);
    m_chatId = data;

/* 
    // ideal approach (to be implemented)

    JsonParser J;
    J.read("dev.Json");

    m_apiEndPoint += J.getValue("api");
    m_apiEndPoint += J.getValue("token");
    m_chatId = J.getValue("chatId");

*/

  } else {
    cout << "fail to open the file that contain token!";
  }
  delete[] data;
  return m_apiEndPoint;
}

// ============================ public method   ================================

Environment::Environment(const string& fileName) {
  // file that log the received resource
  m_logPtr = fopen("log.txt", "a");

  // read api and chatid
  read(fileName);

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

  // set to perform POST
  curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "POST");

  // set detailed error message destination
  curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, m_errBuffer);

  // config libcurl to "tell more"
  curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 0);

  // redirect server response to log file from standard output
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void*)m_logPtr);
}

Environment::~Environment() {
  fclose(m_logPtr);
  curl_easy_cleanup(m_curl);
  curl_global_cleanup();
}

ostream& Environment::errorCheck(ostream& os) const {
  if(m_result != CURLE_OK) {
    cerr << "libcurl: error code (" << m_result << ") ";
    if(m_errBuffer[0]) {
      os << m_errBuffer << endl;
    } else {
      os << curl_easy_strerror(m_result) << endl;
    }
  } else {
    os << "every thing is ok" << endl;
  }
  return os;
}

Environment& Environment::createRequest(const string& method, const string& message) {
  string url;
  string logMsg;

  url += m_apiEndPoint;
  url += method;
  url += '?';

  // form the request body
  url += m_chatId;

  if(method == "sendMessage") {
    url += "&text=";
  } else if(0) {
    // do something
  }

  // escape the illegal characters
  char* escaped = curl_easy_escape(m_curl, message.c_str(), message.length());
  
  if(escaped) {
    url += escaped;
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());

    logMsg = timeStamp() + "Created request " + url + "\n";
    writeToLog(logMsg.c_str());
  } else {
    cout << "fail to create url!" << endl;
  }
  // reclaim resource by easy escape
  curl_free(escaped);

  return *this;
}

CURLcode Environment::addHeaderMsg(const string& msg) {
  return m_result;
}

string Environment::timeStamp() {
  string timeStamp;
  timeStamp += "[";
  timeStamp += __TIMESTAMP__;
  timeStamp += "] ";
  return timeStamp;
}

Environment& Environment::writeToLog(const string& logMsg) {
  fprintf(m_logPtr, "%s", logMsg.c_str());
  return *this;
}

Environment& Environment::execute(ostream& log) {
  string msg = timeStamp();
  msg += "Performed http request => server response ";
  writeToLog(msg);
  m_result = curl_easy_perform(m_curl);
  writeToLog("\n");
  errorCheck();
  return *this;
}
