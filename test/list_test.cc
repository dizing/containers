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
    int a;
    int b;
    testClass() : a(0), b(0) {
        std::cout << "baseConstuctor" << std::endl;
    }

    testClass(int a, int b) : a(a), b(b) {
    
        std::cout << "usual Constuctor" << std::endl;
    }

    testClass(const testClass& other) : a(other.a), b(other.b) {
        std::cout << "copyConstuctor" << std::endl;
    }
};

TEST_F(ListTest, test) {
  list<testClass> c;
  c.CreateNodeByCopy(testClass(1, 2));

}