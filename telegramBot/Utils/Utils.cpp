// Name: On Hei Chau
// Email: ochau2@myseneca.ca
// Student Id: 172917213
// I have done all the coding by myself and only copied the code that my professor
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <regex>
#include "Utils.h"

using namespace std;

namespace sdds {
  Utils U;
  
  Utils& Utils::clearInputBuffer() {
    while (cin.get() != '\n');
    return *this;
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

  bool Utils::strChr(const char& ch, const char* charsList) const {
    bool match{};
    int i{};

    for(i = 0; charsList[i] && !match; i++) {
      if(charsList[i] == ch) {
        match = true;
      }
    }
    return match;
  }

  Utils& Utils::strcpy(char* destination, const char* source) {
    int i{};
    for(i = 0; source[i]; i++) {
      destination[i] = source[i];
    }
    destination[i] = '\0';

    return *this;
	}

  size_t Utils::strlen(const char* string) const {
    size_t i{};

    if(string) {
      while (string[i]) i++;
    }
    return i;
	}

  const char* Utils::strTruncate(char* destination, const char* source, const int truncateLen) {
    int i{};
    for(i = 0; i < truncateLen; i++) {
      destination[i] = source[i];
    }
    destination[i] = '\0';
    return destination;
  }

  int Utils::strcmp(const char* str1, const char* str2) {
    bool same{true};
    int i{};

    while (same && str1[i] && str2[i]) {
      str1[i] == str2[i]? i++ : same = false;
    }
    if(str1[i] != str2[i]) {
      same = false;
    }
    return same? 0 : -999;
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

  char* Utils::requestHeapMemory(const int requestSize) {
    return (new char[requestSize]);
  }

    ostream& Utils::bar(const char ch, const int numberOfChars, ostream& ostr) {
    ostr.fill(ch);
    ostr.width(numberOfChars);
    ostr << "";
    return ostr;
  }

  bool Utils::isPositiveIntInput(const char* usrInput) {
    return (regex_match(usrInput, regex("^[0-9]+$")));
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


  int Utils::getInt(const char* errMsg, const int min, const int max, istream& inputStream, ostream& outputStream) {
    bool valid{};
    int number{};

    while (!valid) {
      inputStream >> number;

      if(inputStream.fail()) { // input format error
        inputStream.clear();
        inputStream.ignore(2000, '\n');
        outputStream << "Invalid Integer, try again: " ;
      } else if(inputStream.peek() == ' ') {
        outputStream << "please do not enter leading white space character: ";
        inputStream.ignore(2000, '\n');
      } else if(inputStream.get() != '\n') {  // user entered extra character
        inputStream.ignore(2000, '\n');
        outputStream << "please enter integer only > ";
      } else if(number < min || number > max) { // the number entered is out of valid range
        outputStream << "[" << min << "<=value<=" << max << "], " << errMsg;
      } else {
        valid = true;
      }
    }
    return number;
  }
/*   int Utils::getInt(int& destination) {
    bool valid{false};
    char temp[100 + 1]{}; 
    regex pattern("^[0-9]+$");


    cin.getline(temp, 101);
	}

  int Utils::getInt(int& destination, const int min, const int max) {

	}
  double Utils::getDouble(double& destination) {

	}

  double Utils::getDouble(double& destination, const double min, const double max) {

	}

  char Utils::getChar(const char* validChars) {

	}

  Utils& Utils::getString(char* destination, const int minLength, const int maxLength) {

	} */

} // namespace sdds

/* using namespace sdds;

int main() {
  int number{};

  cout << "Enter a number between 22 - 99 >";

  number = U.getInt("retry: ", 22, 88);

  cout << "The number is " << number;
  
  return 0;
} */