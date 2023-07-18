#include <initializer_list>
#include <vector>

#include "containers.h"
#include "gtest/gtest.h"

class VectorTest : public ::testing::Test {
 protected:
  VectorTest() {}

  template <typename T>
  static void check_with_std(const vector<T>& vec,
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

TEST_F(VectorTest, test) {
  std::initializer_list list = {1, 2, 3, 4, 5};
  vector<int> vec;
  std::vector<int> std_vec;
  vec.push_back(3);
  vec.push_back(2);
  vec.push_back(2);
  vec.push_back(2);
  vec.push_back(2);
  vec.shrink_to_fit();
  std_vec.push_back(3);
  std_vec.push_back(2);
  std_vec.push_back(2);
  std_vec.push_back(2);
  std_vec.push_back(2);
  std_vec.shrink_to_fit();
  check_with_std(vec, std_vec);

  vector vec_from_list(list.begin(), list.end());
  std::vector std_vec_from_list(list.begin(), list.end());
  check_with_std(vec_from_list, std_vec_from_list);
  vector<double> vec_from_rvalue_list = {2,   32, 243, 4, 2345,  23,
                                         423, 4,  23,  5, 345.34};
  std::vector<double> std_from_rvalue_list = {2,   32, 243, 4, 2345,  23,
                                              423, 4,  23,  5, 345.34};
  check_with_std(vec_from_rvalue_list, std_from_rvalue_list);

  vector vec_copy_constructor(vec_from_rvalue_list);
  std::vector std_vec_copy_constructor(std_from_rvalue_list);
  check_with_std(vec_copy_constructor, std_vec_copy_constructor);
}