#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "Financewatcher.h"
#include "Utils.h"

using namespace std;
using namespace utils;


// read the data from file which record what product to monitor
Financewatcher& Financewatcher::load(const string& fileName) {
  Fincialentity buffer;
  char* inputBuffer{};
  ifstream watchListInput(fileName);

  if(!watchListInput.is_open()) {
    writeToLog(timeStamp()) << "<ERROR> Financewatcher fail to open file. File name: " << fileName << endl;
  } else {
    m_listSize = U.grepDashC(regex(","), watchListInput);
    m_watchList = new Fincialentity[m_listSize];

    // why seekg not work???? have to clear close and open to rewind the char position
    watchListInput.clear();
    watchListInput.close();
    watchListInput.open(fileName);
    U << watchListInput;

    for (size_t i = 0; i < m_listSize; i++) {
      inputBuffer = U.getString(',');
      buffer.m_identifier = static_cast<string>(inputBuffer);
      delete[] inputBuffer;

      inputBuffer = U.getString(',');
      buffer.m_alias = static_cast<string>(inputBuffer);
      delete[] inputBuffer;

      inputBuffer = U.getString(',');
      buffer.m_holdingPosition = stoi(static_cast<string>(inputBuffer));
      delete[] inputBuffer;

      inputBuffer = U.getString('\n');
      buffer.alertPercentage = stod(static_cast<string>(inputBuffer));
      delete[] inputBuffer;

      m_watchList[i] = buffer;
    }
  }
  return *this;
}

Financewatcher& Financewatcher::updateWatchList(Fincialentity* watchList) {
  for (size_t i = 0; i < m_listSize ; i++) {
    updateWatchCase(watchList[i]);
  }
  return *this;
}

Financewatcher& Financewatcher::updateWatchCase(Fincialentity& watchItem) {
  bool ok{};
  string pattern{}, url{"https://ca.finance.yahoo.com/quote/"};
  int attemptCount{}, matchedCount{};
  url += watchItem.m_identifier;

  while (!ok && attemptCount < MAX_ATTEMPT) {
    fetchHtml(url);
    attemptCount++;

    matchedCount += (responseBuffer().find(latestPricePattern(watchItem.m_identifier)) != -1);
    matchedCount += (responseBuffer().find(changePercentagePattern(watchItem.m_identifier)) != -1);
    matchedCount += (responseBuffer().find(previousClosePattern) != -1);

    if(matchedCount == VALID_RESPONSE) {
      ok = true;
    } else {
      matchedCount = 0;
      writeToLog(timeStamp()) << "<WARNING> Financewatcher fetched invalid HTML. Attempt count:  " << attemptCount << endl;
    }
  }

  if(ok) {
    watchItem.m_currentPrice = extractLatestPrice(watchItem.m_identifier);
    watchItem.m_currentChangePercentage = extractChangePercentage(watchItem.m_identifier);
    watchItem.m_previousClose = extractPreviousClose();
  } else { writeToLog(timeStamp()) << "<ERROR> fail to fetch valid data from internet over 5 attempts." << endl;
    writeToLog("See invalid.html for more details") << endl;
    ofstream out("invalid.html");
    out << responseBuffer();
  }
  return *this;
}

string Financewatcher::latestPricePattern(const string& identifier) {
  return static_cast<string>("data-symbol=\"" + identifier + "\" data-test=\"qsp-price\" data-field=\"regularMarketPrice\"");
}

string Financewatcher::changePercentagePattern(const string& identifier) {
  return static_cast<string>("data-symbol=\"" + identifier + "\" data-field=\"regularMarketChangePercent\"");
}

// extract the double value from plain string that return from xxx extractor
double Financewatcher::extractValue(const string& source) {
  string temp{};
  for (size_t i = 0; i < source.length() && (isdigit(source[i]) || source[i] == '.'); i++) {
    temp += source[i];
  }
  return stod(temp);
}

// 
double Financewatcher::extractLatestPrice(const string& identifier) {
  string result{};
  result = responseBuffer().substr(responseBuffer().find(latestPricePattern(identifier)), 150);
  result = result.substr(result.find("value=\"") + 7, 8);
  return extractValue(result);
}

// 
double Financewatcher::extractPreviousClose() {
  string result{};
  result = responseBuffer().substr(responseBuffer().find(previousClosePattern) + previousClosePattern.length(), 10);
  return extractValue(result);
}

//
double Financewatcher::extractChangePercentage(const string& identifier) {
  string result{};
  char ch;
  result = responseBuffer().substr(responseBuffer().find(changePercentagePattern(identifier)) , 500);
  result = result.substr(result.find("value=\"") + 7, 10);
  ch = result[0] == '-'? '-' : '+';

  if(ch == '-') {
    result = result.substr(1, 8);
  }

  return ch == '+'? extractValue(result) * 100 : -extractValue(result) * 100;
}

Financewatcher::Financewatcher(const string& fileName) :BotIO("dev.JSON"){
  load(fileName);
}

Financewatcher::~Financewatcher() {
  delete[] m_watchList;
}

Financewatcher& Financewatcher::run() {
  writeToLog(timeStamp()) << "======================> Start" << endl;
  updateWatchList(m_watchList);
  resourceHandler();
  writeToLog(timeStamp()) << "======================> Exit" << endl << endl;
  return *this;
}

Financewatcher& Financewatcher::resourceHandler() {
  string report{};
  for (size_t i = 0; i < m_listSize; i++) {
    if(m_watchList[i].needNotification()) {
      report = m_watchList[i].createReport();
      writeToLog(timeStamp()) << "ready to send the following msg to tg: " << endl << report << endl;
      sendMessageToTelegram(report);
    } else {
      writeToLog(timeStamp()) << m_watchList[i].m_alias << " no need to send report." << endl;
    }
  }
  return *this;
}