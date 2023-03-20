#define CATCH_CONFIG_MAIN
#include <iostream>
#include <cstring>
#include "catch.hpp"
#include "../Utils.h"

using namespace sdds;
using namespace std;

TEST_CASE("strlen test") {
  CHECK(U.strlen("") == 0);
  CHECK(U.strlen("1") == 1);
  CHECK(U.strlen("1234567890") == 10);
  CHECK(U.strlen("          ") == 10);
  CHECK(U.strlen("2.67846552") == 10);
  CHECK(U.strlen("ajdl(@*& !") == 10);
  CHECK(U.strlen("\t\n12345678") == 10);
}

TEST_CASE("strcpy test") {
  char target[20]{"Good morning!"};
  char empty[20]{0};

  U.strcpy(empty, target);
  CHECK(strcmp(empty, target) == 0);
  CHECK(U.strlen(empty) == 13);

  U.strcpy(empty, "");
  CHECK(strcmp(empty, "") == 0);
  CHECK(U.strlen(empty) == 0);

  U.strcpy(empty, "abcdefg");
  CHECK(strcmp(empty, "abcdefg") == 0);
  CHECK(U.strlen(empty) == 7);

  U.strcpy(empty, "\t\n!@$");
  CHECK(strcmp(empty, "\t\n!@$") == 0);
  CHECK(U.strlen(empty) == 5);
}

TEST_CASE("strcmp test") {
  CHECK(U.strcmp("", "") == 0);
  CHECK(U.strcmp("123", "123") == 0);
  CHECK(U.strcmp("1 a n", "1 a n") == 0);
  CHECK(U.strcmp("a\n2b", "a\n2b") == 0);
  CHECK(U.strcmp("527_dv", "527_dv") == 0);
  CHECK(U.strcmp(" 662.64 ", " 662.64 ") == 0);
  CHECK(U.strcmp("662.64 ", "662.64 ") == 0);
  CHECK(U.strcmp("", "1") != 0);
  CHECK(U.strcmp("123", "123 ") != 0);
  CHECK(U.strcmp(" 123", "123") != 0);
  CHECK(U.strcmp("abc ", "abc  ") != 0);
  CHECK(U.strcmp("good morning!", "good morning!\n") != 0);
  CHECK(U.strcmp("\n ", "\n") != 0);
  CHECK(U.strcmp("\t", "\t ") != 0);
  CHECK(U.strcmp("123asdf", "\tdfsdfsd ") != 0);
}

TEST_CASE("strcat test") {
  char str1[13] = "Good ";
  char str2[] = "morning!";

  U.strcat(str1, str2);

  CHECK(str1[U.strlen(str1)] == '\0');
  CHECK(U.strlen(str1) == 13);
  CHECK(U.strcmp(str1, "Good morning!") == 0);

  char str3[] = "Hi ";
  char str4[] = "there.";
  CHECK(U.strlen(str3) == 3);

  U.strcat(str3, str4);

  CHECK(str3[U.strlen(str3)] == '\0');
  CHECK(U.strlen(str3) == 9);
  CHECK(U.strcmp(str3, "Hi there.") == 0);
}

TEST_CASE("str to int test") {
  CHECK(U.strToPositiveInt("0") == 0);
  CHECK(U.strToPositiveInt("0000000") == 0);
  CHECK(U.strToPositiveInt("0000") == 0);
  CHECK(U.strToPositiveInt("1") == 1);
  CHECK(U.strToPositiveInt("123") == 123);
  CHECK(U.strToPositiveInt("69436") == 69436);
  CHECK(U.strToPositiveInt("10") == 10);
  CHECK(U.strToPositiveInt("100") == 100);
  CHECK(U.strToPositiveInt("101") == 101);
  CHECK(U.strToPositiveInt("00500") == 500);
  CHECK(U.strToPositiveInt("6042") == 6042);
  CHECK(U.strToPositiveInt("00500") == 500);
}
