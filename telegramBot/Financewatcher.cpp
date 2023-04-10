
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