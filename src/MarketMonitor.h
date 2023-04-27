#ifndef FINANCEWATCHER_H__
#define FINANCEWATCHER_H__
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include "BotIO.h"
#include "MarketAsset.h"

const int VALID_RESPONSE = 3;
const int MAX_ATTEMPT = 5;

const std::string previousClosePattern = "PREV_CLOSE-value\">";

class MarketMonitor : public BotIO {
private:
  MarketAsset* m_usMarketWatchList{};
  MarketAsset* m_asiaMarketWatchList{};
  size_t m_usListSize{};
  size_t m_asiaListSize{};
  std::unordered_map<std::string, std::function<void()>> m_callbackMap;

  // loads data from file to retrieve the watch list
  MarketMonitor& load(const std::string& fileName);

  // updates the information of all items in the watch list
  MarketMonitor& updateWatchList(MarketAsset* watchList, const int size);

  // updates one item's state with latest market information
  MarketMonitor& updateWatchCase(MarketAsset& watchItem);

  // returns a string that uniquely identify the html element that contains price info
  std::string latestPricePattern(const std::string& identifier);

  // returns a string that uniquely identify the html element that contains percentage change info
  std::string changePercentagePattern(const std::string& identifier);

  // extract the double value from plain string that come from html element
  double extractValue(const std::string& source);

  // extract price info from html element string
  double extractLatestPrice(const std::string& identifier);

  // extract previous close info from html element string
  double extractPreviousClose();

  // extract percentage change info from html element string
  double extractChangePercentage(const std::string& identifier);

public:
  // constructs the instance's watch list according to the file name provided
  MarketMonitor(const std::string& fileName, const std::string& token, const std::string& chatId);
  ~MarketMonitor();

  std::unordered_map<std::string, std::function<void()>>& callbackMap();
  
  void sendUsMarketDayReport_cb();

  void sendAsiaMarketDayReport_cb();

  void monitorUsMarketPrice_cb();

  void monitorAsiaMarketPrice_cb();
};




#endif