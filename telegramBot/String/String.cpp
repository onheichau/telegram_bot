#include <iostream>
#include <fstream>
#include <regex>
#include "String.h"
#include "../Utils/Utils.h"

using namespace std;

namespace sdds {
  
  // private method

  String& String::strcat(const String& source) {
    String concatenatedOutcome;
    concatenatedOutcome.m_length = m_length + source.m_length;
    concatenatedOutcome.m_data = new char[concatenatedOutcome.m_length + 1];
    concatenatedOutcome.m_data[0] = '\0';

    U.strcat(concatenatedOutcome.m_data, m_data).strcat(concatenatedOutcome.m_data, source.m_data);

    *this = concatenatedOutcome;
    return *this;
  }

  // public method
  String::String(const char* str) {
    m_data = nullptr;
    m_length = 0;

    if(str != nullptr) {
      m_length = strlen(str);
      m_data = new char[m_length + 1];
      U.strcpy(m_data, str);
    }
	}

  String::String(const String& source) {
    m_data = nullptr;
    m_length = 0;
    *this = source;
  }

  String& String::operator=(const String& source) {
    if(this != &source) {
      m_length = source.m_length;
      delete[] m_data;

      if(source.m_data != nullptr) {
        m_data = new char[source.m_length + 1];
        strcpy(m_data, source.m_data);
      } else {
        m_data = nullptr;
      }
    }
    return *this;
  }

  String::~String() {
    delete[] m_data;
	}


  std::istream& String::inputWholeFileFrom(std::istream& inputStream) {
    while (!inputStream.eof() && inputStream.rdstate() == ios::eofbit && (*this += inputStream.get()));
    m_inputWholeFile = false;
    return inputStream;
  }
  
  String& String::setImportWholeFile() {
    m_inputWholeFile = true;
    return *this;
  }
  // ========================== quires ====================================

  // return the length of string
  size_t String::length() const {
    return m_length;
	}

  // return true if string is in safe empty state
  bool String::isEmpty() const {
    return (m_data == nullptr && m_length == 0);
  }

  bool String::isNextInputWholeFile() const {
    return m_inputWholeFile;
  }
  // ========================== binary operator overload ==================

  // perform string concatenation and return the concatenated string
  String String::operator+(const String& source) const {
    String concatenatedString;
    
    concatenatedString.strcat(*this).strcat(source);

    return concatenatedString;
  }

  bool String::operator==(const String& rightOperhand) const {
    bool equal{};

    if(m_data == nullptr && rightOperhand.m_data == nullptr) {
      equal = true;
    } else if(U.strcmp(m_data, rightOperhand.m_data) == 0) {
      equal = true;
    }
    return equal;
  }
  // ========================== unary operator overload ====================

  // current string concatenated with string after += operator
  String& String::operator+=(const String& source) {
    strcat(source);
    return *this;
  }

  // current string concatenated with character after += operator
  String& String::operator+=(const char ropChar) {
    String tem{"1"};
    tem.m_data[0] = ropChar;
    strcat(tem);
    return *this;
	}

  // ==========================  conversion =================================

  // return a C style character pointer
  String::operator const char* () const {
    return m_data;
  }

  // return true if String holding heap resource
  String::operator bool() const {
    return !isEmpty();
	}

  // return an integer if string is int convertible
  String::operator int() const {
    int value{};
    bool negative = (m_data[0] == '-');
    int i{(int)m_length - 1}, j{};


    if(negative) {
      while (i >= 1) {
        value += (m_data[i--] - 48) * U.power(10, j++);
      }
    } else {
      while (i >= 0) {
        value += (m_data[i--] - 48) * U.power(10, j++);
      }
    }
    return negative? -value : value;
  }


/*   bool String::intConvertible() {
    return (regex_match((const char*)*this, regex("^-?[0-9]+$")));
  } */


  std::ostream& String::display(std::ostream& os) const {
    os << m_data;
    return os;
	}


  // ================================ helper function ===========================
  ostream& operator<<(ostream& ostr, String& str) {
    return (str.display(ostr));
  }

  istream& operator>>(istream& inputStream, String& rhs) {
    char ch{0};

    if(rhs.isNextInputWholeFile()) {
      rhs.inputWholeFileFrom(inputStream);
    } 
    else {
      while(ch != '\n') {
        ch = inputStream.get();
        
        if(ch != '\n') {
          rhs += ch;
        }
      }
    }
    return inputStream;
  }
} // namespace sdds

