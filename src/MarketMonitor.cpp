#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "MarketMonitor.h"
#include "Utils.h"

using namespace std;
using namespace utils;


// read the data from file which record what product to monitor
MarketMonitor& MarketMonitor::load(const string& fileName) {
  MarketAsset buffer;
  char* inputBuffer{};
  ifstream watchListInput(fileName);

  if(!watchListInput.is_open()) {
    writeToLog(timeStamp()) << "<ERROR> MarketMonitor fail to open file. File name: " << fileName << endl;
  } else {
    m_usListSize = U.grepDashC(regex("MARKET_U"), watchListInput);
    m_usMarketWatchList = new MarketAsset[m_usListSize];

    m_asiaListSize = U.grepDashC(regex("MARKET_A"), watchListInput);
    m_asiaMarketWatchList = new MarketAsset[m_asiaListSize];

    U << watchListInput;

    size_t usListIdxPosition{};
    size_t asiaListIdxPosition{};
    for (size_t i = 0; i < (m_usListSize + m_asiaListSize); i++) {
      inputBuffer = U.getString(',');
      buffer.m_identifier = static_cast<string>(inputBuffer);
      delete[] inputBuffer;

      inputBuffer = U.getString(',');
      buffer.m_alias = static_cast<string>(inputBuffer);
      delete[] inputBuffer;

      inputBuffer = U.getString(',');
      buffer.m_holdingPosition = stoi(static_cast<string>(inputBuffer));
      delete[] inputBuffer;

      inputBuffer = U.getString(',');
      buffer.alertPercentage = stod(static_cast<string>(inputBuffer));
      delete[] inputBuffer;

      inputBuffer = U.getString('\n');
      if(U.strcmp(inputBuffer, "MARKET_U") == 0) {
        m_usMarketWatchList[usListIdxPosition++] = buffer;
      } else if(U.strcmp(inputBuffer, "MARKET_A") == 0) {
        m_asiaMarketWatchList[asiaListIdxPosition++] = buffer;
      } else {
        U.strcmp(inputBuffer, "MARKET_A");
        writeToLog(timeStamp()) << "<ERROR> detected invalid market identifier [" << inputBuffer << "] in " << fileName << endl;
      }
      delete[] inputBuffer;
    }
  }
  return *this;
}

MarketMonitor& MarketMonitor::updateWatchList(MarketAsset* watchList, const int size) {
  for (size_t i = 0; i < size ; i++) {
    updateWatchCase(watchList[i]);
  }
  return *this;
}

MarketMonitor& MarketMonitor::updateWatchCase(MarketAsset& watchItem) {
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
      writeToLog(timeStamp()) << "<WARNING> MarketMonitor fetched invalid HTML. Attempt count:  " << attemptCount << endl;
    }
  }

  if(ok) {
    watchItem.m_currentPrice = extractLatestPrice(watchItem.m_identifier);
    watchItem.m_currentChangePercentage = extractChangePercentage(watchItem.m_identifier);
    watchItem.m_previousClose = extractPreviousClose();
  } else { 
    writeToLog(timeStamp()) << "<ERROR> fail to fetch valid data from internet over 5 attempts." << endl;
    writeToLog("See invalid.html for more details") << endl;
    ofstream out("invalid.html");
    out << responseBuffer();
  }
  return *this;
}

string MarketMonitor::latestPricePattern(const string& identifier) {
  return static_cast<string>("data-symbol=\"" + identifier + "\" data-test=\"qsp-price\" data-field=\"regularMarketPrice\"");
}

string MarketMonitor::changePercentagePattern(const string& identifier) {
  return static_cast<string>("data-symbol=\"" + identifier + "\" data-field=\"regularMarketChangePercent\"");
}

// extract the double value from plain string that return from xxx extractor
double MarketMonitor::extractValue(const string& source) {
  string temp{};
  for (size_t i = 0; i < source.length() && (isdigit(source[i]) || source[i] == '.'); i++) {
    temp += source[i];
  }
  return stod(temp);
}

// 
double MarketMonitor::extractLatestPrice(const string& identifier) {
  string result{};
  result = responseBuffer().substr(responseBuffer().find(latestPricePattern(identifier)), 150);
  result = result.substr(result.find("value=\"") + 7, 8);
  return extractValue(result);
}

// 
double MarketMonitor::extractPreviousClose() {
  string result{};
  result = responseBuffer().substr(responseBuffer().find(previousClosePattern) + previousClosePattern.length(), 10);
  return extractValue(result);
}

//
double MarketMonitor::extractChangePercentage(const string& identifier) {
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

MarketMonitor::MarketMonitor(const string& fileName, const string& token, const string& chatId) 
:BotIO(token, chatId)
{
  load(fileName);
  m_callbackMap["UsMonitor"] = [this]() { monitorUsMarketPrice_cb(); };
  m_callbackMap["AsiaMonitor"] = [this]() { monitorAsiaMarketPrice_cb(); };
  m_callbackMap["UsReport"] = [this]() { sendUsMarketDayReport_cb(); };
  m_callbackMap["AsiaReport"] = [this]() { sendAsiaMarketDayReport_cb(); };
}

MarketMonitor::~MarketMonitor() {
  delete[] m_usMarketWatchList;
  delete[] m_asiaMarketWatchList;
}

unordered_map<string, function<void()>>& MarketMonitor::callbackMap() {
  return m_callbackMap;
}

void MarketMonitor::sendUsMarketDayReport_cb() {
    string report;

    report += "============================ Daily Report On US Market ============================\n";
    updateWatchList(m_usMarketWatchList, m_usListSize);

    for (size_t i = 0; i < m_usListSize; i++) {
      MarketAsset& watchItem = m_usMarketWatchList[i];
      report += "Stock code: ";
      report += watchItem.m_alias;
      report += " change %: ";
      report += to_string(watchItem.m_currentChangePercentage);
      report += " change on portfolio: ";
      report += to_string(watchItem.m_holdingPosition * watchItem.m_previousClose * watchItem.m_currentChangePercentage * 0.01);
      report += "\n";
    }
    report += "============================  End of US Market Report  ============================\n";
    sendMessageToTelegram(report);
}

void MarketMonitor::sendAsiaMarketDayReport_cb() {
  string report;
  updateWatchList(m_asiaMarketWatchList, m_usListSize);
  for (size_t i = 0; i < m_asiaListSize; i++) {
    // do some thing to gather data?
  }
  sendMessageToTelegram(report);
}

void MarketMonitor::monitorUsMarketPrice_cb() {
  updateWatchList(m_usMarketWatchList, m_usListSize);
  for (size_t i = 0; i < m_usListSize; i++) {
    if(m_usMarketWatchList[i].needNotification()) {
      // do something 
    }
  }
}

void MarketMonitor::monitorAsiaMarketPrice_cb() {
  updateWatchList(m_asiaMarketWatchList, m_asiaListSize);
  for (size_t i = 0; i < m_asiaListSize; i++) {
    if(m_asiaMarketWatchList[i].needNotification()) {
      // do something 
    }
  }
}
