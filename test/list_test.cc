#include <list>

#include "containers.h"
#include "gtest/gtest.h"
class ListTest : public ::testing::Test {
 protected:
  ListTest() {}

  std::list<int> a = {1, 2, 3};
  list<int> b = list<int>();
};

struct testClass {
  std::string a;
  std::string b;
  testClass() : a(""), b("") { std::cout << "baseConstuctor" << std::endl; }

  testClass(const std::string& a, const std::string& b) : a(a), b(b) {
    std::cout << "usualConstuctor" << std::endl;
  }

  testClass(const testClass& other) : a(other.a), b(other.b) {
    std::cout << "copyConstuctor" << std::endl;
  }

  testClass(testClass&& other) : a(std::move(other.a)), b(std::move(other.b)) {
    std::cout << "moveConstuctor" << std::endl;
  }
  bool operator==(const testClass& other) const {
    return other.a == a && other.b == b;
  }
};

TEST_F(ListTest, test) {
  testClass name = testClass("Maron", "Kubanov");
  std::cout << "----" << std::endl;
  std::list<testClass> c;
  c.push_back(testClass("1", "2"));
  c.push_back(name);
  c.push_front(testClass("me", "me"));
  c.insert(++c.begin(), testClass("you", "you"));
  // c.push_back("bbbb", "aaa");
  for (auto& element : c) {
    std::cout << element.a << std::endl;
  }
  std::cout << "----" << std::endl;
  list<testClass> c1;
  c1.push_back(testClass("1", "2"));
  c1.push_back(name);
  c1.push_front(testClass("me", "me"));
  c1.insert(++c1.begin(), testClass("you", "you"));
  // c.push_back("bbbb", "aaa");
  for (auto& element : c1) {
    std::cout << element.a << std::endl;
  }
  EXPECT_EQ(c1.front(), c.front());
  EXPECT_EQ(c1.back(), c.back());
}