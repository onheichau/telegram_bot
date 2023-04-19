#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "BotIO.h"
#include "Timer.h"
#include "Utils.h"
#include "Environment.h"

using namespace std;
using namespace utils;

static Environment m_env;
static string m_apiEndPoint;
static string m_chatId;

BotIO::BotIO(const string &token, const string &chatId) {
  m_apiEndPoint = "https://api.telegram.org/";
  m_apiEndPoint += token;
  m_apiEndPoint += "/";

  m_chatId = "chat_id=-";
  m_chatId += chatId;
  }

string BotIO::timeStamp()
{
  return timer.timeStamp();
}

ofstream& BotIO::writeToLog(const string& message) {
  return m_env.writeToLog(message);
}

const string BotIO::responseBuffer() {
  return m_env.responseBuffer();
}

const string& BotIO::fetchHtml(const string& url) {
  m_env.clearResponseBuffer().setOutputToBuffer().createRequest(url);
  m_env << timeStamp() << "<fetch HTML>" << endl;
  m_env.execute();
  return m_env.responseBuffer();
}

BotIO& BotIO::sendMessageToTelegram(const string& msg) {
  string request = m_apiEndPoint;
  request += "sendMessage?";
  request += m_chatId;
  request += "&text=";
  request += m_env.escapeString(msg);
  request += "&parse_mode=Markdown";
  m_env.setOutputToLog().createRequest(request);
  writeToLog(timeStamp()) << "<call telegram send message method>"  << endl;
  m_env.execute();
  return *this;
}