#include <list>

#include "containers.h"
#include "gtest/gtest.h"

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

class ListTest : public ::testing::Test {
 protected:
  ListTest() {}

  std::list<testClass> stdll = std::list<testClass>();
  list<testClass> ll = list<testClass>();
};

TEST_F(ListTest, Modifiers) {
  testClass name = testClass("Maron", "Kubanov");
  stdll.push_back(testClass("1", "2"));
  stdll.push_back(name);
  stdll.push_front(testClass("me", "me"));
  stdll.insert(++ ++stdll.begin(), testClass("you", "you"));
  stdll.insert(stdll.begin(), testClass("they", "are"));
  stdll.emplace_back();
  ll.push_back(testClass("1", "2"));
  ll.push_back(name);
  ll.push_front(testClass("me", "me"));
  ll.insert(++ ++ll.begin(), testClass("you", "you"));
  ll.insert(ll.begin(), testClass("they", "are"));
  ll.emplace_back();
  EXPECT_EQ(stdll.size(), ll.size());
  auto ll_it = ll.begin();
  auto stdll_it = stdll.begin();
  for (std::size_t i = 0; i < ll.size(); ++i) {
    EXPECT_EQ(*ll_it, *stdll_it);
    ++ll_it;
    ++stdll_it;
  }
}

TEST_F(ListTest, test) {
  // const std::list<testClass> stdl = {{"a", "b"}, {"c", "d"}};
  // std::cout << "----" << std::endl;
  // const list<testClass> l = {{"a", "b"}, {"c", "d"}};
}