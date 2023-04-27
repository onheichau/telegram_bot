#ifndef FINCIALENTITY_H__
#define FINCIALENTITY_H__
#include <string>

struct MarketAsset {
  std::string m_identifier;
  std::string m_alias;
  double m_currentPrice{};
  double m_currentChangePercentage{};
  double m_previousClose{};
  int m_holdingPosition;
  double alertPercentage;
  bool marketIsOpen{};
  bool needNotification();
  std::string createReport();
  std::string createAlert();
};

#endif
