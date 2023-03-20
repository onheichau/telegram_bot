#ifndef SDDS_STRING_H
#define SDDS_STRING_H
#include <iostream>

namespace sdds {

  class String {
    // data members
    char* m_data;
    size_t m_length;
    bool m_inputWholeFile{};

    String& strcat(const String& source);

  public:
    // constructor
    String(const char* str = nullptr);

    // copy constructor
    String(const String& source);

    // copy assignment operator
    String& operator=(const String& source);

    // destructor
    ~String();

    // set next input operation on ifstream to input full file content
    std::istream& inputWholeFileFrom(std::istream& inputStream);

    String& setImportWholeFile();

    // ========================== quires ====================================

    // return the length of string
    size_t length() const;

    // return true if string is in safe empty state
    bool isEmpty() const;

    // return true if next operation on ifstream is set to input full file
    bool isNextInputWholeFile() const;

    //
    bool intConvertible() const;

    // ========================== binary operator overload ==================

    // perform string concatenation and return the concatenated string
    String operator+(const String& source) const;

    // return true if two string is equal
    bool operator==(const String& rightOperhand) const;

  // ========================== unary operator overload ====================

    // current string concatenated with string after += operator
    String& operator+=(const String& source);

    // current string concatenated with character after += operator
    String& operator+=(const char ropChar);

  // ==========================  conversion =================================

    // return a C style character pointer
    operator const char* () const;

    // return true if String holding heap resource
    operator bool() const;

    // return an integer if string is int convertible
    operator int() const;


/*     operator double() const;
    bool positiveDoubleConvertible() const;
    bool negativeDoubleConvertible() const;  */
    std::ostream& display(std::ostream& os = std::cout) const;
  };

  // ================================ helper function ===========================
  std::ostream& operator<<(std::ostream& ostr, String& str);
  
  //
  std::istream& operator>>(std::istream& inputStream, String& rhs);
  
} // namespace sdds

#endif