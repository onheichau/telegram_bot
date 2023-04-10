#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <regex>
#include <fstream>
#include "Utils.h"

using namespace std;

namespace utils {

  Utils::Utils() {
    m_inputPtr = &cin;
    m_outputPtr = &cout;
  }
  
  
  char* Utils::getString(const char delimiter, const char* errorMsg , const char* promptMsg, const int maxLength) {
    bool ok{};
    istream& istream = *m_inputPtr;
    char* inputBuf = new char[2];
    inputBuf[0] = '\0';

    while (!ok) {
      istream.getline(inputBuf, 2, '\n');

      if(istream.good() && inputBuf[0]) { // user enter 1 char
        ok = true;
      } else if(istream.good() && !inputBuf[0]) { // user only press enter
        ok = true;
        delete[] inputBuf;
        inputBuf = nullptr;
      } else {
        istream.clear();
      }

      if(!ok) {
        while (istream.good() && !istream.eof() && istream.peek() != delimiter) {
          dynCat(inputBuf, (istream.get()));
        }
        istream.ignore(); // drop delimeter

        if((int)(U.strlen(inputBuf)) > maxLength) {
          cout << errorMsg << promptMsg;
        } else {
          ok = true;
        }
      }
    }
    return inputBuf;
  }

  Utils& Utils::dynCat(char*& destination, const char ch) {
    int len = strlen(destination);
    char* temp{};

    if(destination == nullptr) {
      destination = new char[2];
      destination[0] = ch;
      destination[1] = '\0';
    } 
    else {
      temp = new char[len + 2];
      strcpy(temp, destination);
      temp[len] = ch;
      temp[len + 1] = '\0';

      deAloClone(destination, temp);
    }
    delete[] temp;
    return *this;
  }
  char Utils::getChar(const char* validCharList, const char* errorMsg, const char* promptMsg) {
    bool valid{};
    char ch{};
    istream& istream = *m_inputPtr;
    ostream& ostream = *m_outputPtr;

    while (!valid) {
      istream >> ch;
      if (istream.fail() || istream.get() != '\n' || !strchr(ch, validCharList)) {
        istream.clear();
        istream.ignore(2000, '\n');
        ostream << errorMsg << promptMsg;
      } else {
        valid = true;
      }
    }
    return ch;
  }

  // >>>>>>>>>>>>>>>>>>>>>>>>> string method <<<<<<<<<<<<<<<<<<<<<<<<<<<<
  
  size_t Utils::strlen(const char* string) const {
    size_t i = 0;

    if(string != nullptr) {
      while (string[i]) i++;
    }

    return i;
	}

  Utils& Utils::strTruncate(char* destination, const char* source, const int truncateLen) {
    int i{};
    for(i = 0; i < truncateLen; i++) {
      destination[i] = source[i];
    }
    destination[i] = '\0';
    return *this;
  }

  Utils& Utils::strcat(char* destination, const char* source) {
    int i{}, j{};
    int destNullTermIndex = U.strlen(destination);
    int srcStrLength = U.strlen(source);

    for(i = destNullTermIndex; i < destNullTermIndex + srcStrLength; i++, j++) {
      destination[i] = source[j];
    }

    destination[i] = '\0';
    return *this;
  }

  Utils& Utils::strcpy(char* destination, const char* source) {
    int i{};
    for(i = 0; source[i]; i++) {
      destination[i] = source[i];
    }
    destination[i] = '\0';
    return *this;
	}

  Utils& Utils::strncpy(char* destination, const char* source, const int size) {
    int i {};
    for (i = 0; i < size && source[i]; i++) {
      destination[i] = source[i];
    }
    destination[i] = '\0';
    return *this;
  }

  bool Utils::strchr(const char ch, const char* charsList) const {
    bool chExistInList{false};
    int i{};

    for(i = 0; charsList[i] != '\0' && !chExistInList; i++) {
      if(charsList[i] == ch) {
        chExistInList = true;
      }
    }
    return chExistInList;
  }

  int Utils::strcmp(const char* str1, const char* str2) {
    int result{};
    int i{};

    while (str1[i] && str2[i] && result == 0) {
      if(str1[i] > str2[i]) {
        result = 1;
      } else if (str1[i] < str2[i]) {
        result = -1;
      }
      i++;
    }

    if(str1[i] && !str2[i]) { // string 1 still have char, str2 null terminated
      result = 1;
    } else if(!str1[i] && str2[i]) { // string 1 null terminated, str2 still have char
      result = -1;
    }

    return result;
  }

  // return 1 if str1 is before ch2
  int Utils::strAlphaOrderCmp(const char* str1, const char* str2) {
    int result{};
    size_t str1Len = U.strlen(str1);
    size_t str2Len = U.strlen(str2);
    size_t minLen = str1Len;
    if (str2Len < str1Len) {
      minLen = str2Len;
    }

    for(size_t i = 0; i < minLen && !result; i++) {
      result = charCmp(str1[i], str2[i]);
    }
    return result;
  }

  // >>>>>>>>>>>>>>>>>>>> format displayer <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  ostream& Utils::print(const char* str, const int width, const bool leftJustify, const char filling)const {
    ostream& os = *m_outputPtr;
    os.width(width);
    os.fill(filling);
    if (leftJustify) {
      os.setf(ios::left);
    }
    os << str;
    os.fill(' ');
    if (leftJustify) {
      os.unsetf(ios::left);
    }
    return os;
  } 

  ostream& Utils::print(const int value, const int width, const bool leftJustify, const char filling)const{
    ostream& os = *m_outputPtr;
    os.width(width);
    os.fill(filling);
    if (leftJustify) {
      os.setf(ios::left);
    }
    os << value;
    os.fill(' ');
    if (leftJustify) {
      os.unsetf(ios::left);
    }
    return os;
  }

  ostream& Utils::print(const double value, const int precision, const int width, const bool leftJustify, const char filling)const {
    ostream& os = *m_outputPtr;
    os.width(width);
    os.fill(filling);
    os.setf(ios::fixed);
    os.precision(precision);
    if (leftJustify) {
      os.setf(ios::left);
    }
    os << value;
    os.fill(' ');
    os.unsetf(ios::fixed);
    if (leftJustify) {
      os.unsetf(ios::left);
    }
    return os;
  }

  // >>>>>>>>>>>>>>>>>>>>>>>>>> DMA related <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  
  Utils& Utils::deAloClone(char*& destination, const char* source) {
    delete[] destination;
    destination = nullptr;
    if(source) {
      destination = new char[U.strlen(source) + 1];
      U.strcpy(destination, source);
    }
    return *this;
  }

  Utils& Utils::dynConcat(char*& destination, const char* source) {
    size_t totalLen;
    char* temp{};

    if(destination) {
      totalLen = strlen(destination) + strlen(source);
    } else {
      totalLen = strlen(source);
    }

    temp = new char[totalLen + 1];
    temp[0] = '\0';

    if(destination) {
      strcat(temp, destination).strcat(temp, source);
    } else {
      strcat(temp, source);
    }

    deAloClone(destination, temp);
    
    delete[] temp;
    return *this;
  }

  Utils& Utils::dynConcat(char*& destination, const char ch) {
    char tempChar[2]{ch};
    tempChar[1] = '\0';
    dynConcat(destination, tempChar);
    return *this;
  }
  // >>>>>>>>>>>>>>>>>>>>>>>>>> math <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  
  // return 1 if ch1 is before ch2 in the alphabetic hierarchy, 0 denotes the same, -1 ch2 is before ch1 case insensitive
  int Utils::charCmp(char ch1, char ch2) {
    int result{};

    if (ch1 > ch2) {
      result = 1;
    } else if (ch1 < ch2) {
      result = -1;
    }
    return result;
  }

  long Utils::power(long base, long exponent) {
    double result = 1;
    while (exponent != 0) {
      if (exponent % 2 == 1) {
        result *= base;
      }
      base *= base;
      exponent /= 2;
    }
    return result;
  }

  unsigned int Utils::strToPositiveInt(const char* intStr) {
    int strLength = int(U.strlen(intStr));
    int i{};
    int sum{};

    while (strLength - 1 >= 0) {
      sum += (intStr[--strLength] - 48 ) * power(10, i++);
    }
    return sum;
  }


  bool Utils::inRangeOf(const double value, const double min, const  double max) {
    bool inRange = (value >= min && value <= max);
    return inRange;
  }

  // operator overload
  Utils& Utils::operator<<(std::ostream& ostream) {
    m_outputPtr = &ostream;
    return *this;
  }
  
  Utils& Utils::operator<<(std::istream& istream) {
    m_inputPtr = &istream;
    return *this;
  }

  Utils::operator bool() {
    return true;
  }

  // regex 

  string Utils::grep(const regex& pattern, const string& data) {
    string result{};
    int startPosition{};
    int endPosition = data.find('\n');
    string subStr{};

    if(endPosition != string::npos) {
      subStr = data.substr(0, endPosition + 1);
    }

    while (endPosition != string::npos) {
      if(regex_search(subStr, pattern)) {
        result += subStr;
      }
      // start at next line first char
      startPosition = endPosition + 1;
      
      // find the next newline char
      endPosition = data.find('\n', startPosition);

      // renew the substr to check
      if(endPosition != string::npos) {
        subStr = data.substr(startPosition, (endPosition - startPosition + 1));
      } else {
        subStr = data.substr(startPosition, data.length() - startPosition);
        if(regex_search(subStr, pattern)) {
          result += subStr;
        }
      }
    }

    subStr = data.substr(startPosition, data.length() - startPosition);

    if(regex_search(subStr, pattern)) {
      result += subStr;
    }

    return result;
  }

  string Utils::grep(const regex& pattern, istream& inputFile) {
    string result{};
    string lineContext{};
    while (inputFile.good()) {
      getline(inputFile, lineContext);
      if(regex_search(lineContext, pattern)) {
        result += lineContext;
        result += '\n';
      }
    }
    return result;
  }

  int Utils::grepDashC(const regex& pattern, istream& inputFile) {
    int count{};
    string S;

    while (inputFile.good() && !inputFile.eof()) {
      getline(inputFile, S);
      count += regex_search(S, pattern);
    }
    return count;
  }

  int Utils::grepDashC(const regex& pattern , const string& inputString) {
    int count{};
    int startPosition{};
    int endPosition = inputString.find('\n');
    string subStr{};

    // there is 1 line / empty str
    if(endPosition == string::npos) {
      count = regex_search(inputString, pattern);
    } else { // start extract first line
      subStr = inputString.substr(0, endPosition);
    }

    while (endPosition != string::npos) {
      count += regex_search(subStr, pattern);

      // start at next line first char
      startPosition = endPosition + 1;

      // find the next newline char
      endPosition = inputString.find('\n', startPosition);

      // renew the substr to check
      if(endPosition != string::npos) {
        subStr = inputString.substr(startPosition, endPosition - startPosition);
      }
    }
    return count;
  }

  Utils U;

} // namespace utils

using namespace utils;

/* int main() { */
/*   // test on grep(regex, string)
  string data;
  string result;

  data = "Hi there\nToronto\nHongKong\nHong Kong Toronto!!!!\n --->Toronto";
  result = U.grep(regex("Toronto"), data);

  cout << "The result is: ";
  cout << result << endl; */

  // test on grep -c
/*   ifstream foo("grep-c.txt");
  int recordCount = U.grepDashC(regex(","), foo);
  cout << recordCount << endl;
  return 0;
} */