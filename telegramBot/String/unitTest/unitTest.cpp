#define CATCH_CONFIG_MAIN
#include <iostream>
#include "catch.hpp"
#include "../../awsomePlumberP26ProToolBox/Utils.h"
#include "../String.h"

using namespace std;
using namespace sdds;

TEST_CASE("string constructor test") {
  String a;
  String b{"Hi there! greetings!"};
  String c{nullptr};

  CHECK(a.length() == 0);
  CHECK(b.length() == U.strlen("Hi there! greetings!"));
  CHECK(c.length() == 0);
  CHECK(a.isEmpty());
  CHECK(b.isEmpty() == false);
  CHECK(c.isEmpty());
}

TEST_CASE("operator = with Cstring test") {
  String a = "Good morning";
  CHECK(U.strcmp((const char*)a, "Good morning") == 0);
  CHECK(U.strcmp((const char*)a, "") != 0);
  CHECK(U.strcmp((const char*)a, "Good morning ") != 0);
  CHECK(U.strcmp((const char*)a, " Good morning") != 0);

  a = "content changed";
  CHECK(U.strcmp((const char*)a, "content changed") == 0);
  CHECK(U.strcmp((const char*)a, "") != 0);
  CHECK(U.strcmp((const char*)a, "content changed ") != 0);
  CHECK(U.strcmp((const char*)a, " content changed") != 0);
}

TEST_CASE("operator = with String object test") {
  String a = "no time to die";
  String b{};
  String tester{nullptr};

  CHECK(!a.isEmpty());
  CHECK(b.isEmpty());
  CHECK(tester.isEmpty());

  tester = (b = a);

  CHECK(!a.isEmpty());
  CHECK(!b.isEmpty());
  CHECK(!tester.isEmpty());

  CHECK(U.strlen((const char*)a) == U.strlen((const char*)b));
  CHECK(U.strlen((const char*)a) == U.strlen((const char*)tester));
  CHECK(a.length() == b.length());
  CHECK(b.length() == tester.length());
  CHECK(U.strcmp((const char*)a, (const char*)b) == U.strcmp((const char*)b, (const char*)tester));
  
  a = nullptr;
  tester = (b = a);

  CHECK(a.isEmpty());
  CHECK(b.isEmpty());
  CHECK(tester.isEmpty());
}

TEST_CASE("operator = with normal Cstring test") {
  char message[30] = "I go to school by bus!!!";
  size_t messageLen = U.strlen(message);
  String tester = "1234567890";

  CHECK(tester.isEmpty() == false);
  CHECK(tester.length() == 10);

  tester = nullptr;
  CHECK(tester.isEmpty() == true);
  CHECK(tester.length() == 0);
  
  tester = message;
  CHECK(tester.isEmpty() == false);
  CHECK(tester.length() == messageLen);

  tester = "12345";
  CHECK(tester.isEmpty() == false);
  CHECK(tester.length() == 5);
}

TEST_CASE("concat test") {
  String test;

  test += "123";

  CHECK(U.strcmp((const char*)test, "123") == 0);

  test = "Good morning";

  test += "!#@$";

  CHECK(U.strcmp((const char*)test, "Good morning!#@$") == 0);

  test = nullptr;

  CHECK(test.isEmpty());

  String container;
  test = "123";
  String test2 = "456";

  container = test + test2;

  String container1 = test + test2;

  CHECK(U.strcmp((const char*)test, "123") == 0);
  CHECK(U.strcmp((const char*)test2, "456") == 0);
  CHECK(U.strcmp((const char*)container, "123456") == 0);
  CHECK(U.strcmp((const char*)container1, "123456") == 0);
  CHECK(container == container1);
  CHECK_FALSE(test == test2);
}

TEST_CASE("String convert to int") {
  String a = "123";
  String b = "-58";

  CHECK((int)a == 123);
  CHECK((int)b == -58);
}