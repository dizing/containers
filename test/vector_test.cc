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
  vector<int> vec;
  std::vector<int> std_vec;
  vec.push_back(3);
  vec.push_back(2);
  std_vec.push_back(3);
  std_vec.push_back(2);
  check_with_std(vec, std_vec);
}