// Name: On Hei Chau
// Email: ochau2@myseneca.ca
// Student Id: 172917213
// I have done all the coding by myself and only copied the code that my professor
#ifndef SDDS_UTILS_H
#define SDDS_UTILS_H

#include <iostream>

// Welcome!

namespace sdds {

  class Utils {
  public:
    Utils& clearInputBuffer();
    long power(long base, long exponent);
    bool strChr(const char& ch, const char* charsList) const;
    Utils& strcpy(char* destination, const char* source);
    const char* strTruncate(char* destination, const char* source, const int truncateLen);
    size_t strlen(const char* string) const;
    int strcmp(const char* str1, const char* str2);
    Utils& strcat(char* destination, const char* source);
    bool strchr(const char ch, const char* charsList) const;
    std::ostream& bar(const char ch, const int numberOfChars = 1, std::ostream& ostr = std::cout);
    char* requestHeapMemory(const int requestSize);
    bool isPositiveIntInput(const char* usrInput);
    unsigned int strToPositiveInt(const char* intStr);
    int getInt(const char* errmsg, const int min = INT32_MIN, const int max = INT32_MAX, 
               std::istream& inputStream = std::cin, std::ostream& outputStream = std::cout);
/*     int getInt(int& destination);
    int getInt(int& destination, const int min, const int max); // min max inclusive
    double getDouble(double& destination);
    double getDouble(double& destination, const double min, const double max); // min max inclusive
    char getChar(const char* validChars);
    Utils& getString(char* destination, const int minLength, const int maxLength); */
  };
  extern Utils U;

} // namespace sdds



#endif