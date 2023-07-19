#include "test_class.h"

testClass::testClass() : a(""), b("") {}

testClass::testClass(const std::string& a, const std::string& b) : a(a), b(b) {
  // std::cout << "usualConstuctor" << std::endl;
}
testClass::testClass(const testClass& other) : a(other.a), b(other.b) {
  // std::cout << "copyConstuctor" << std::endl;
}

testClass::testClass(testClass&& other)
    : a(std::move(other.a)), b(std::move(other.b)) {
  // std::cout << "moveConstuctor" << std::endl;
}
bool testClass::operator==(const testClass& other) const {
  return other.a == a && other.b == b;
}
testClass& testClass::operator=(const testClass& other) {
  a = other.a;
  b = other.b;
  return *this;
}
std::ostream& operator<<(std::ostream& stream, const testClass& test_obj) {
  std::cout << test_obj.a << " " << test_obj.b << std::endl;
  return stream;
}