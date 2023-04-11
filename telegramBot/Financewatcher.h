#ifndef FINANCEWATCHER_H__
#define FINANCEWATCHER_H__
#include <iostream>
#include <string>
#include "BotIO.h"
#include "Fincialentity.h"

const int VALID_RESPONSE = 3;
const int MAX_ATTEMPT = 5;

const std::string previousClosePattern = "PREV_CLOSE-value\">";

class Financewatcher : public BotIO {
private:
  Fincialentity* m_watchList{};
  size_t m_listSize{};

  // loads data from file to retrieve the watch list
  Financewatcher& load(const std::string& fileName);

  // updates the information of all items in the watch list
  Financewatcher& updateWatchList(Fincialentity* watchList);

  // updates one item's state with latest market information
  Financewatcher& updateWatchCase(Fincialentity& watchItem);

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
  Financewatcher(const std::string& fileName);
  ~Financewatcher();

  // program entry point
  Financewatcher& run();

  // distinguishes if messages is needed to send. If yes, it will handle the operation
  Financewatcher& resourceHandler();
};




#endif