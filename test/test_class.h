#if !defined(CONTAINERS_TEST_TEST_CLASS_H)
#define CONTAINERS_TEST_TEST_CLASS_H

#include <iostream>

struct testClass {
  std::string a;
  std::string b;
  testClass();
  testClass(const std::string& a, const std::string& b);
  testClass(const testClass& other);
  testClass(testClass&& other);
  bool operator==(const testClass& other) const;
  testClass& operator=(const testClass& other);
};
std::ostream& operator<<(std::ostream& stream, const testClass& test_obj);
#endif  // CONTAINERS_TEST_TEST_CLASS_H
