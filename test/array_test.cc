#include "containers.h"
#include "gtest/gtest.h"

class ArrayTest : public ::testing::Test {
 protected:
  ArrayTest() {
    std::for_each(std_base_constructor.begin(), std_base_constructor.end(),
                  [](auto &n) {
                    static int i = 0;
                    n = i++;
                  });
    std::for_each(base_constructor.begin(), base_constructor.end(),
                  [](auto &n) {
                    static int i = 0;
                    n = i++;
                  });
  }
  std::array<double, 6> std_arr = {1, 2, 3, 4};
  array<double, 6> arr = {1, 2, 3, 4};
  std::array<std::string, 6> std_arr2 = {"232", "232", "232", "232"};
  array<std::string, 6> arr2 = {"232", "232", "232", "232"};
  const std::array<double, 6> std_const_arr = {1, 2, 3, 4};
  const array<double, 6> const_arr = {1, 2, 3, 4};
  std::array<const std::vector<int>, 2> std_arr_const_vec = {
      {{1, 2, 3}, {1, 2, 3}}};
  array<const std::vector<int>, 2> arr_const_vec = {{{1, 2, 3}, {1, 2, 3}}};
  std::array<int, 10> std_base_constructor = std::array<int, 10>();
  array<int, 10> base_constructor = array<int, 10>();
  array<int, 0> zero_arr = array<int, 0>();
  std::array<int, 0> std_zero_arr = std::array<int, 0>();
};

TEST_F(ArrayTest, element_access) {
  EXPECT_EQ(arr[3], std_arr[3]);
  EXPECT_EQ(arr.at(3), std_arr.at(3));
  EXPECT_THROW(arr.at(7), std::out_of_range);
  EXPECT_EQ(arr.front(), std_arr.front());
  EXPECT_EQ(arr.back(), std_arr.back());
  EXPECT_EQ(arr.data()[0], std_arr.data()[0]);
  EXPECT_EQ(std::is_const_v<std::remove_reference_t<decltype(arr[3])>>, false);
  EXPECT_EQ(std::is_const_v<std::remove_reference_t<decltype(arr.at(3))>>,
            false);
  EXPECT_EQ(const_arr[3], std_const_arr[3]);
  EXPECT_EQ(const_arr.at(3), std_const_arr.at(3));
  EXPECT_THROW(const_arr.at(7), std::out_of_range);
  EXPECT_EQ(const_arr.data()[0], std_const_arr.data()[0]);
  EXPECT_EQ(std::is_const_v<std::remove_reference_t<decltype(const_arr[3])>>,
            true);
  EXPECT_EQ(std::is_const_v<std::remove_reference_t<decltype(const_arr.at(3))>>,
            true);
  EXPECT_NE(arr.begin(), arr.end());
  EXPECT_EQ(zero_arr.begin(), zero_arr.end());
}

TEST_F(ArrayTest, capacity) {
  EXPECT_EQ(arr.size(), 6);
  EXPECT_EQ(arr.size(), arr.max_size());
  EXPECT_EQ(arr.empty(), false);
  EXPECT_EQ(std_zero_arr.size(), zero_arr.size());
}

TEST_F(ArrayTest, modifiers) {
  arr.fill(2.3);
  std_arr.fill(2.3);
  for (size_t i = 0; i < std_arr.size(); ++i) {
    EXPECT_EQ(arr[i], std_arr[i]);
  }

  array<double, 6> arr_analog = {4, 3, 2, 1};
  arr_analog.swap(arr);
  for (size_t i = 0; i < 4; ++i) {
    EXPECT_DOUBLE_EQ(arr_analog[i], 2.3);
    EXPECT_EQ(arr[i], 4 - i);
  }

  array<std::string, 6> arr2_analog = {"new", "new", "new", "new"};
  arr2_analog.swap(arr2);
  for (size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(arr2_analog[i], "232");
    EXPECT_EQ(arr2[i], "new");
  }
}