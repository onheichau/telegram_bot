#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "BotIO.h"
#include "Utils.h"

using namespace std;
using namespace utils;

static Environment m_env;
static string m_apiEndPoint;
static string m_chatId;

// return a string that contain api, token and chatid in url format
BotIO& BotIO::read(const string& fileName) {
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

  } else {
    writeToLog(timeStamp()) << "BotIO fail to open file. File name: " << fileName << endl;
  }
  delete[] data;
  return *this;
}

BotIO::BotIO(const std::string fileName) {
  read(fileName);
}

string BotIO::timeStamp() {
  return m_env.timeStamp();
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
  m_env.setOutputToLog().createRequest(request);
  writeToLog(timeStamp()) << "<call telegram send message method>"  << endl;
  m_env.execute();
  return *this;
}

BotIO& BotIO::run(){
  return *this;
}

BotIO& BotIO::resourceHandler() {
  return *this;
}
