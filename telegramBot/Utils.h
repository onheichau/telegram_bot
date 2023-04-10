#ifndef UTILS_UTILS_H__
#define UTILS_UTILS_H__
#include <iostream>
#include <regex>

namespace utils {

  class Utils {
    std::ostream* m_outputPtr{};
    std::istream* m_inputPtr{};
  public:
    Utils();


    /// @brief a fool-proof data extraction from cin
    /// @tparam Type any primitive type such as int , long , double....
    /// @param min the minimum value to validate
    /// @param max the maximum value to validate
    /// @param formatErrMsg message to print when user entered invalid data format
    /// @param promptMsg the message to print when user entered out of range value
    /// @return validated, in range of min and max value
    template<typename Type>
    Type getNumber(const Type min, const Type max, const char* formatErrMsg = "", const char* promptMsg = "") {
      bool valid{};
      Type number{};

      while (!valid) {
        std::cin >> number;

        if(std::cin.fail()) { // input format error
          std::cin.clear();
          std::cin.ignore(2000, '\n');
          std::cout << formatErrMsg;
        } else if(std::cin.get() != '\n') {  // user entered extra character
          std::cin.ignore(2000, '\n');
          std::cout << formatErrMsg;
        } else if(number < min || number > max) { // the number entered is out of valid range
          std::cout << "[" << min << "<=value<=" << max << "]" << promptMsg;
        } else {
          valid = true;
        }
      }
      return number;
    }


    /// @brief 
    /// @tparam Type 
    /// @param min 
    /// @param max 
    /// @param genericErrMsg 
    /// @param promptMsg 
    /// @return 
    template<typename Type>
    Type getNumber2(const Type min, const Type max, const char* genericErrMsg = "", const char* promptMsg = "") {
      bool valid{};
      Type number{};

      while (!valid) {
        std::cin >> number;

        if(std::cin.fail()) { // input format error
          std::cin.clear();
          std::cin.ignore(2000, '\n');
          std::cout << genericErrMsg << promptMsg;
        } else if(std::cin.get() != '\n') {  // user entered extra character
          std::cin.ignore(2000, '\n');
          std::cout << genericErrMsg << promptMsg;
        } else if(number < min || number > max) { // the number entered is out of valid range
          std::cout << genericErrMsg << promptMsg;
        } else {
          valid = true;
        }
      }
      return number;
    }

    /// @brief a and b must support copy assign operator overload
    /// @tparam Type 
    template<typename Type>
    void swap(Type& a, Type& b) {
      Type c;
      c = a;
      a = b;
      b = c;
    }

    char* getString(const char delimiter, const char* errorMsg = "" , const char* promptMsg = "", const int maxLength = 9999);
    Utils& dynCat(char*& destination, const char ch);
    char getChar(const char* validCharList, const char* errorMsg = "", const char* promptMsg = "");

    // string related   
    size_t strlen(const char* string) const;
    Utils& strTruncate(char* destination, const char* source, const int truncateLen);
    Utils& strcat(char* destination, const char* source);
    Utils& strcpy(char* destination, const char* source);
    Utils& strncpy(char* destination, const char* source, const int size);
    bool strchr(const char ch, const char* charsList) const;
    int strcmp(const char* str1, const char* str2);
    int strAlphaOrderCmp(const char* str1, const char* str2);

    // format displayer
    std::ostream& print(const char* str, const int width, const bool leftJustify=false, const char filling=' ')const;
    std::ostream& print(const int value, const int width, const bool leftJustify=false, const char filling=' ')const;
    std::ostream& print(const double value, const int precision, const int width, const bool leftJustify=false, const char filling=' ')const;

    // DMA related
    Utils& deAloClone(char*& destination, const char* source);
    Utils& dynConcat(char*& destination, const char* source);
    Utils& dynConcat(char*& destination, const char ch);

    
    // return 1 if ch1 is before ch2 in the alphabetic hierarchy, 0 denotes the same, -1 ch2 is before ch1 case insensitive
    int charCmp(char ch1, char ch2);
    

    //math
    long power(long base, long exponent);
    unsigned int strToPositiveInt(const char* intStr);
    bool inRangeOf(const double value, const double min, const double max);

    // operator overload
    Utils& operator<<(std::ostream& ostream);
    Utils& operator<<(std::istream& istream);
    operator bool();

    // regex 

    std::string grep(const std::regex& pattern, std::istream& inputFile);
    std::string grep(const std::regex& pattern, const std::string& data);

    // perform grep -c like operation
    int grepDashC(const std::regex& pattern , std::istream& inputFile);
    int grepDashC(const std::regex& pattern , const std::string& inputString);
  };
  
  extern Utils U;

} // namespace sdds



#endif
