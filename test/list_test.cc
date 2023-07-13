#include <list>

#include "containers.h"
#include "gtest/gtest.h"

struct testClass {
  std::string a;
  std::string b;
  testClass() : a(""), b("") {  // std::cout << "baseConstuctor" << std::endl;
  }

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

  const std::list<testClass> const_std_list = {{"a", "b"}, {"c", "d"}};
  const list<testClass> const_list = {{"a", "b"}, {"c", "d"}};

  template <typename T>
  static void check_with_std(const list<T>& ll, const std::list<T>& stdll) {
    EXPECT_EQ(ll.size(), stdll.size());
    auto ll_it = ll.begin();
    auto stdll_it = stdll.begin();
    while(ll_it != ll.end()) {
      EXPECT_EQ(*ll_it, *stdll_it);
      ++ll_it;
      ++stdll_it;
    }
  }
};

TEST_F(ListTest, Constructors) { 
  check_with_std(ll_il, stdll_il);
  check_with_std(const_list, const_std_list); 
  }

TEST_F(ListTest, OneElementModifiers) {
  EXPECT_EQ(ll.size(), stdll.size());
  EXPECT_EQ(ll.empty(), stdll.empty());
  testClass name = testClass("Maron", "Kubanov");
  stdll.push_back(testClass("1", "2"));
  stdll.push_back(name);
  stdll.push_front(testClass("me", "me"));
  stdll.push_front(name);
  stdll.pop_back();
  stdll.insert(++ ++stdll.begin(), testClass("you", "you"));
  stdll.insert(stdll.begin(), testClass("they", "are"));
  stdll.pop_front();
  stdll.emplace_back();
  stdll.emplace_back("323", "323");
  stdll.erase(-- --stdll.end());
  ll.push_back(testClass("1", "2"));
  ll.push_back(name);
  ll.push_front(testClass("me", "me"));
  ll.push_front(name);
  ll.pop_back();
  ll.insert(++ ++ll.begin(), testClass("you", "you"));
  ll.insert(ll.begin(), testClass("they", "are"));
  ll.pop_front();
  ll.emplace_back();
  ll.emplace_back("323", "323");
  ll.erase(-- --ll.end());
  check_with_std(ll, stdll);
}

TEST_F(ListTest, ElementAccess) {
  EXPECT_EQ(ll_il.front(), stdll_il.front());
  EXPECT_EQ(ll_il.back(), stdll_il.back());
}

TEST_F(ListTest, swap) {
  list<testClass> list_for_swap = {{"1", "2"}, {"31", "23"}, {"12", "22"}};
  std::list<testClass> stdlist_for_swap = {{"1", "2"}, {"31", "23"}, {"12", "22"}};

  list<testClass> list_for_swap2 = {{"dasd", "sad"}, {"gdf", "asda"}};
  std::list<testClass> stdlist_for_swap2 = {{"dasd", "sad"}, {"gdf", "asda"}};

  list<testClass> list_for_swap3; // empty
  std::list<testClass> stdlist_for_swap3; // empty

  list_for_swap.swap(list_for_swap2);
  stdlist_for_swap.swap(stdlist_for_swap2);
  list_for_swap.push_back(testClass("1", "2")); // check that all work accordingly
  stdlist_for_swap.push_back(testClass("1", "2")); // check that all work accordingly
  check_with_std(list_for_swap, stdlist_for_swap);
  check_with_std(list_for_swap2, stdlist_for_swap2);

  list_for_swap.swap(list_for_swap3);
  stdlist_for_swap.swap(stdlist_for_swap3);
  list_for_swap.push_back(testClass("1", "2")); // check that all work accordingly
  stdlist_for_swap.push_back(testClass("1", "2")); // check that all work accordingly
  check_with_std(list_for_swap, stdlist_for_swap);
  check_with_std(list_for_swap3, stdlist_for_swap3);
}

