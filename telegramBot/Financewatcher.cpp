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
  Finicalentity buffer;
  char* inputBuffer{};
  ifstream watchListInput(fileName);

  if(!watchListInput.is_open()) {
    writeToLog(timeStamp()) << "<ERROR> Financewatcher fail to open file. File name: " << fileName << endl;
  } else {
    m_listSize = U.grepDashC(regex(","), watchListInput);
    m_watchList = new Finicalentity[m_listSize];

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

Financewatcher& Financewatcher::updateWatchList(Finicalentity* watchList) {
  for (size_t i = 0; i < m_listSize ; i++) {
    updateWatchCase(watchList[i]);
  }
  return *this;
}

Financewatcher& Financewatcher::updateWatchCase(Finicalentity& watchItem) {
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
  string pattern = "data-symbol=\"" + identifier + "\" data-test=\"qsp-price\" data-field=\"regularMarketPrice\"";
  return pattern;
}

string Financewatcher::changePercentagePattern(const string& identifier) {
  string pattern = "data-symbol=\"" + identifier + "\" data-field=\"regularMarketChangePercent\"";
  return pattern;
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
  for (size_t i = 0; i < m_listSize; i++) {
    if(/* m_watchList[i].m_currentChangePercentage > m_watchList[i].alertPercentage || 
      -m_watchList[i].m_currentChangePercentage > m_watchList[i].alertPercentage) */
      m_watchList[i].m_currentPrice > 3) {
        string message;
        message += "[Alert] => ";
        message += m_watchList[i].m_alias;
        message += "\n";
        message += "current price: ";
        message += to_string(m_watchList[i].m_currentPrice);
        message += "\n";
        message += "change percentage: ";
        message += to_string(m_watchList[i].m_currentChangePercentage);
        message += "\n";
        message += "previous close: ";
        message += to_string(m_watchList[i].m_previousClose);
        message += "\n";
        message += "holding position: ";
        message += to_string(m_watchList[i].m_holdingPosition);
        message += "\n";
        message += "change on portfolio: ";
        message += to_string(m_watchList[i].m_holdingPosition * m_watchList[i].m_previousClose * m_watchList[i].m_currentChangePercentage * 0.01);
        message += "\n";
        writeToLog(timeStamp()) << "ready to send the following msg to tg: " << endl << message << endl;
      sendMessageToTelegram(message);
    }
  }
  return *this;
}