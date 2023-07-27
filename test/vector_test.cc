#include <initializer_list>
#include <vector>

#include "containers.h"
#include "gtest/gtest.h"
#include "test_class.h"

class VectorTest : public ::testing::Test {
 protected:
  VectorTest() {}

  template <typename T>
  static void check_with_std(const dizing::vector<T>& vec,
                             const std::vector<T>& std_vec) {
    EXPECT_EQ(vec.size(), std_vec.size());
    EXPECT_EQ(vec.capacity(), std_vec.capacity());
    auto it = vec.begin();
    auto std_it = std_vec.begin();
    for (size_t i = 0; i < std_vec.size(); ++i) {
      EXPECT_EQ(*it, *std_it);
      ++it;
      ++std_it;
    }
  }
};

TEST_F(VectorTest, ConstructorTest) {
  std::initializer_list list = {1, 2, 3, 4, 5};
  dizing::vector vec_from_list(list.begin(), list.end());
  std::vector std_vec_from_list(list.begin(), list.end());
  check_with_std(vec_from_list, std_vec_from_list);
  dizing::vector<double> vec_from_rvalue_list = {2,   32, 243, 4, 2345,  23,
                                         423, 4,  23,  5, 345.34};
  std::vector<double> std_from_rvalue_list = {2,   32, 243, 4, 2345,  23,
                                              423, 4,  23,  5, 345.34};
  check_with_std(vec_from_rvalue_list, std_from_rvalue_list);

  dizing::vector vec_copy_constructor(vec_from_rvalue_list);
  std::vector std_vec_copy_constructor(std_from_rvalue_list);
  check_with_std(vec_copy_constructor, std_vec_copy_constructor);

  dizing::vector vec_move_constructor(std::move(vec_copy_constructor));
  std::vector std_vec_move_constructor(std::move(std_from_rvalue_list));
  check_with_std(vec_copy_constructor, std_vec_copy_constructor);
  check_with_std(vec_move_constructor, std_vec_move_constructor);
}

TEST_F(VectorTest, ElementAccess) {
  dizing::vector<testClass> vec = {{"12", "12"}, {"22", "234"}, {"ddd", "aaa"}};
  std::vector<testClass> std_vec = {
      {"12", "12"}, {"22", "234"}, {"ddd", "aaa"}};
  check_with_std(vec, std_vec);
  EXPECT_EQ(vec.front(), std_vec.front());
  EXPECT_EQ(vec.back(), std_vec.back());
  EXPECT_EQ(vec[1], std_vec[1]);
  EXPECT_EQ(vec.at(1), std_vec.at(1));
  EXPECT_THROW(vec.at(30), std::out_of_range);
  EXPECT_EQ(std::is_const_v<std::remove_reference_t<decltype(vec.at(3))>>,
            false);
  EXPECT_EQ(std::is_const_v<std::remove_reference_t<decltype(vec[3])>>, false);

  // CONST ELEMENT ACCESS
  const dizing::vector<testClass> const_vec = {
      {"Alfa", "Omega"}, {"Delta", "Theta"}, {"union", "except"}};
  const std::vector<testClass> const_std_vec = {
      {"Alfa", "Omega"}, {"Delta", "Theta"}, {"union", "except"}};
  check_with_std(vec, std_vec);
  EXPECT_EQ(const_vec.front(), const_std_vec.front());
  EXPECT_EQ(const_vec.back(), const_std_vec.back());
  EXPECT_EQ(const_vec[1], const_std_vec[1]);
  EXPECT_EQ(const_vec.at(1), const_std_vec.at(1));
  EXPECT_THROW(const_vec.at(30), std::out_of_range);
  EXPECT_EQ(std::is_const_v<std::remove_reference_t<decltype(const_vec.at(3))>>,
            true);
  EXPECT_EQ(std::is_const_v<std::remove_reference_t<decltype(const_vec[3])>>,
            true);
}

TEST_F(VectorTest, Modifiers) {
  dizing::vector<testClass> vec = {
      {"Alfa", "Omega"}, {"Delta", "Theta"}, {"union", "except"}};
  std::vector<testClass> std_vec = {
      {"Alfa", "Omega"}, {"Delta", "Theta"}, {"union", "except"}};
  check_with_std(vec, std_vec);
  // PUSH_BACK
  vec.push_back(testClass("323", "232"));
  std_vec.push_back(testClass("323", "232"));
  testClass temp = {"323", "232"};
  vec.push_back(temp);
  std_vec.push_back(temp);
  check_with_std(vec, std_vec);

  // INSERT
  auto vec_end = vec.end();
  vec.insert(--vec_end, temp);
  std_vec.insert(--std_vec.end(), temp);
  vec.insert(vec.end(), temp);
  std_vec.insert(std_vec.end(), temp);
  vec.insert(vec.begin(), testClass("Nano", "Macro"));
  std_vec.insert(std_vec.begin(), testClass("Nano", "Macro"));
  check_with_std(vec, std_vec);

  // ERASE
  vec.erase(vec.begin());
  std_vec.erase(std_vec.begin());
  check_with_std(vec, std_vec);

  // POP_BACK
  vec.pop_back();
  std_vec.pop_back();
  check_with_std(vec, std_vec);

  // CLEAR
  vec.clear();
  std_vec.clear();
  check_with_std(vec, std_vec);
}
