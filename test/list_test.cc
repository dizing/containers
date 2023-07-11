#include <list>

#include "containers.h"
#include "gtest/gtest.h"

struct testClass {
  std::string a;
  std::string b;
  testClass() : a(""), b("") { std::cout << "baseConstuctor" << std::endl; }

  testClass(const std::string& a, const std::string& b) : a(a), b(b) {
    // std::cout << "usualConstuctor" << std::endl;
  }

  testClass(const testClass& other) : a(other.a), b(other.b) {
    // std::cout << "copyConstuctor" << std::endl;
  }

  testClass(testClass&& other) : a(std::move(other.a)), b(std::move(other.b)) {
    // std::cout << "moveConstuctor" << std::endl;
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
  std::list<testClass> stdll_il = {{"first", "second"},
                                   {"firstfirst", "secondsecond"}};
  list<testClass> ll_il = {{"first", "second"}, {"firstfirst", "secondsecond"}};

  template <typename T>
  static void check_with_std(const list<T>& ll, const std::list<T>& stdll) {
    EXPECT_EQ(ll.size(), stdll.size());
    auto ll_it = ll.begin();
    auto stdll_it = stdll.begin();
    for (std::size_t i = 0; i < ll.size(); ++i) {
      EXPECT_EQ(*ll_it, *stdll_it);
      ++ll_it;
      ++stdll_it;
    }
  }
};

TEST_F(ListTest, Constructors) { check_with_std(ll_il, stdll_il); }

TEST_F(ListTest, OneElementModifiers) {
  EXPECT_EQ(ll.size(), stdll.size());
  EXPECT_EQ(ll.empty(), stdll.empty());
  testClass name = testClass("Maron", "Kubanov");
  stdll.push_back(testClass("1", "2"));
  stdll.push_back(name);
  stdll.push_front(testClass("me", "me"));
  stdll.insert(++ ++stdll.begin(), testClass("you", "you"));
  stdll.insert(stdll.begin(), testClass("they", "are"));
  stdll.emplace_back();
  stdll.emplace_back("323", "323");
  ll.push_back(testClass("1", "2"));
  ll.push_back(name);
  ll.push_front(testClass("me", "me"));
  ll.insert(++ ++ll.begin(), testClass("you", "you"));
  ll.insert(ll.begin(), testClass("they", "are"));
  ll.emplace_back();
  ll.emplace_back("323", "323");
  check_with_std(ll, stdll);
}

TEST_F(ListTest, size) {
  std::cout << ll.max_size() << std::endl << stdll.max_size() << std::endl;
  list<testClass> ll_for_swap;  // = {{"1", "2"}, {"3", "4"}};
  ll_for_swap.push_back(testClass("12", "12"));
  ll.swap(ll_for_swap);
  // std::swap(ll, ll_for_swap);
  for (auto& i : ll) {
    std::cout << i.a << i.b << std::endl;
  }

  // for(auto it = ll.begin(); it != )
}