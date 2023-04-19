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
    m_usListSize = U.grepDashC(regex("MARKET_U"), watchListInput);
    m_usMarketWatchList = new Fincialentity[m_usListSize];
    watchListInput.clear();
    watchListInput.close();
    watchListInput.open(fileName);

    m_asiaListSize = U.grepDashC(regex("MARKET_A"), watchListInput);
    m_asiaMarketWatchList = new Fincialentity[m_asiaListSize];
    watchListInput.clear();
    watchListInput.close();
    watchListInput.open(fileName);

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

Financewatcher& Financewatcher::updateWatchList(Fincialentity* watchList, const int size) {
  for (size_t i = 0; i < size ; i++) {
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

Financewatcher::Financewatcher(const string& fileName, const string& token, const string& chatId) 
:BotIO(token, chatId)
{
  load(fileName);
}

Financewatcher::~Financewatcher() {
  delete[] m_usMarketWatchList;
  delete[] m_asiaMarketWatchList;
}

void Financewatcher::sendUsMarketDayReport_cb() {
  string report;
  writeToLog(timeStamp()) << "======================> Start" << endl;

  report += "============================ Daily Report On US Market ============================\n";
  updateWatchList(m_usMarketWatchList, m_usListSize);

  for (size_t i = 0; i < m_usListSize; i++) {
    Fincialentity& watchItem = m_usMarketWatchList[i];
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
  writeToLog(timeStamp()) << "======================> Exit" << endl << endl;
}

void Financewatcher::sendAsiaMarketDayReport_cb() {
  string report;
  writeToLog(timeStamp()) << "======================> Start" << endl;
  updateWatchList(m_asiaMarketWatchList, m_usListSize);
  for (size_t i = 0; i < m_asiaListSize; i++) {
    // do some thing to gather data?
  }
  sendMessageToTelegram(report);
  writeToLog(timeStamp()) << "======================> Exit" << endl << endl;
}

void Financewatcher::monitorUsMarketPrice_cb() {
  writeToLog(timeStamp()) << "======================> Start" << endl;
  updateWatchList(m_usMarketWatchList, m_usListSize);
  for (size_t i = 0; i < m_usListSize; i++) {
    if(m_usMarketWatchList[i].needNotification()) {
      // do something 
    }
  }
  writeToLog(timeStamp()) << "======================> Exit" << endl << endl;
}

void Financewatcher::monitorAsiaMarketPrice_cb() {
  writeToLog(timeStamp()) << "======================> Start" << endl;
  updateWatchList(m_asiaMarketWatchList, m_asiaListSize);
  for (size_t i = 0; i < m_asiaListSize; i++) {
    if(m_asiaMarketWatchList[i].needNotification()) {
      // do something 
    }
  }
  writeToLog(timeStamp()) << "======================> Exit" << endl << endl;
}

/* int main() {
  Financewatcher F("watch.csv");
  F.sendUsMarketDayReport_cb();
  return 0;
} */