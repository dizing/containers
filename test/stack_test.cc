#include <list>
#include <stack>

#include "containers.h"
#include "gtest/gtest.h"
#include "test_class.h"

class StackTest : public ::testing::Test {
 protected:
  StackTest() {}

  template <typename T, template <typename> class ContainerFirst,
            template <typename> class ContainerSecond>
  static void check_with_std(dizing::stack<T, ContainerFirst<T>> st,
                             std::stack<T, ContainerSecond<T>> stdst) {
    EXPECT_EQ(st.size(), stdst.size());
    while (stdst.size() > 0) {
      EXPECT_EQ(st.top(), stdst.top());
      st.pop();
      stdst.pop();
    }
  }
};

TEST_F(StackTest, ListBase) {
  testClass name("Maron", "Kubanov");
  dizing::stack<testClass> a = {
      {"Alfa", "Omega"}, {"Delta", "Theta"}, {"union", "except"}};
  std::stack b(std::list<testClass>{
      {"Alfa", "Omega"}, {"Delta", "Theta"}, {"union", "except"}});
  check_with_std<testClass, dizing::list, std::list>(a, b);
  a.push(name);
  b.push(name);
  check_with_std<testClass, dizing::list, std::list>(a, b);
  a.push(testClass("232", "3232"));
  b.push(testClass("232", "3232"));
  check_with_std<testClass, dizing::list, std::list>(a, b);
  dizing::stack for_swap(
      dizing::list<testClass>{{"1", "2"}, {"3", "4"}, {"5", "6"}});
  std::stack std_for_swap(
      std::list<testClass>{{"1", "2"}, {"3", "4"}, {"5", "6"}});
  for_swap.swap(a);
  std_for_swap.swap(b);
  check_with_std<testClass, dizing::list, std::list>(a, b);
  check_with_std<testClass, dizing::list, std::list>(for_swap, std_for_swap);
}

TEST_F(StackTest, VectorBase) {
  testClass name("Maron", "Kubanov");
  dizing::stack a(dizing::vector<testClass>{
      {"Alfa", "Omega"}, {"Delta", "Theta"}, {"union", "except"}});
  std::stack b(std::vector<testClass>{
      {"Alfa", "Omega"}, {"Delta", "Theta"}, {"union", "except"}});
  check_with_std<testClass, dizing::vector, std::vector>(a, b);
  a.push(name);
  b.push(name);
  check_with_std<testClass, dizing::vector, std::vector>(a, b);
  a.push(testClass("232", "3232"));
  b.push(testClass("232", "3232"));
  check_with_std<testClass, dizing::vector, std::vector>(a, b);
  dizing::stack for_swap(
      dizing::vector<testClass>{{"1", "2"}, {"3", "4"}, {"5", "6"}});
  std::stack std_for_swap(
      std::vector<testClass>{{"1", "2"}, {"3", "4"}, {"5", "6"}});
  for_swap.swap(a);
  std_for_swap.swap(b);
  check_with_std<testClass, dizing::vector, std::vector>(a, b);
  check_with_std<testClass, dizing::vector, std::vector>(for_swap,
                                                         std_for_swap);
}