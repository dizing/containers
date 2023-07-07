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
    testClass() : a(""), b("") {
        std::cout << "baseConstuctor" << std::endl;
    }

    testClass(const std::string & a, const std::string & b) : a(a), b(b) {
    
        std::cout << "usualConstuctor" << std::endl;
    }

    testClass(const testClass& other) : a(other.a), b(other.b) {
        std::cout << "copyConstuctor" << std::endl;
    }

    testClass(testClass&& other) : a(std::move(other.a)), b(std::move(other.b)) {
        std::cout << "moveConstuctor" << std::endl;
    }
};

TEST_F(ListTest, test) {
  list<testClass> c;
  testClass name = testClass("c", "d");
  auto n1 = c.CreateNode(testClass("1", "2"));
  c.InsertNode(n1, name);
  c.InsertNode(static_cast<ListNode<testClass>*>(n1->next), "bbbb", "aaa");
  for(int i = 0; i < 3; ++i) {
    std::cout << n1->value_.a << std::endl;
    n1 = static_cast<ListNode<testClass>*>(n1->next);
  }
}