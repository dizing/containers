#include <list>

#include "containers.h"
#include "gtest/gtest.h"
#include "test_class.h"

class ListTest : public ::testing::Test {
 protected:
  ListTest() {}

  std::list<testClass> stdll = std::list<testClass>();
  dizing::list<testClass> ll = dizing::list<testClass>();
  std::list<testClass> stdll_il = {{"first", "second"},
                                   {"firstfirst", "secondsecond"}};
  dizing::list<testClass> ll_il = {{"first", "second"}, {"firstfirst", "secondsecond"}};

  const std::list<testClass> const_std_list = {{"a", "b"}, {"c", "d"}};
  const dizing::list<testClass> const_list = {{"a", "b"}, {"c", "d"}};

  template <typename T>
  static void check_with_std(const dizing::list<T>& ll, const std::list<T>& stdll) {
    EXPECT_EQ(ll.size(), stdll.size());
    auto ll_it = ll.begin();
    auto stdll_it = stdll.begin();
    for (size_t i = 0; i < stdll.size(); ++i) {
      EXPECT_EQ(*ll_it, *stdll_it);
      ++ll_it;
      ++stdll_it;
    }
  }
};

TEST_F(ListTest, Constructors) {
  check_with_std(ll_il, stdll_il);
  check_with_std(const_list, const_std_list);

  dizing::list from_copy_constructor = ll_il;
  std::list std_from_copy_constructor = stdll_il;
  check_with_std(from_copy_constructor, std_from_copy_constructor);

  dizing::list from_move_constructor = std::move(from_copy_constructor);
  std::list std_from_move_constructor = std::move(std_from_copy_constructor);
  check_with_std(from_move_constructor, std_from_move_constructor);
  check_with_std(from_copy_constructor,
                 std_from_copy_constructor);  // check that values stolen

  dizing::list<testClass> assign = {{"12", "12"}};
  std::list<testClass> std_assign = {{"12", "12"}};

  assign = ll_il;
  std_assign = stdll_il;
  check_with_std(assign, std_assign);
  assign = dizing::list<testClass>({{"12", "12"}, {"22", "22"}});
  std_assign = std::list<testClass>({{"12", "12"}, {"22", "22"}});
  check_with_std(assign, std_assign);
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

TEST_F(ListTest, swap) {
  dizing::list<testClass> list_for_swap = {{"1", "2"}, {"31", "23"}, {"12", "22"}};
  std::list<testClass> stdlist_for_swap = {
      {"1", "2"}, {"31", "23"}, {"12", "22"}};

  dizing::list<testClass> list_for_swap2 = {{"dasd", "sad"}, {"gdf", "asda"}}; 
  std::list<testClass> stdlist_for_swap2 = {{"dasd", "sad"}, {"gdf", "asda"}};

  dizing::list<testClass> list_for_swap3;          // empty
  std::list<testClass> stdlist_for_swap3;  // empty

  list_for_swap.swap(list_for_swap2);
  stdlist_for_swap.swap(stdlist_for_swap2);
  list_for_swap.push_back(
      testClass("1", "2"));  // check that all work accordingly
  stdlist_for_swap.push_back(
      testClass("1", "2"));  // check that all work accordingly
  check_with_std(list_for_swap, stdlist_for_swap);
  check_with_std(list_for_swap2, stdlist_for_swap2);

  list_for_swap.swap(list_for_swap3);
  stdlist_for_swap.swap(stdlist_for_swap3);
  list_for_swap.push_back(
      testClass("1", "2"));  // check that all work accordingly
  stdlist_for_swap.push_back(
      testClass("1", "2"));  // check that all work accordingly
  check_with_std(list_for_swap, stdlist_for_swap);
  check_with_std(list_for_swap3, stdlist_for_swap3);
}

TEST_F(ListTest, ElementAccess) {
  EXPECT_EQ(ll_il.back(), stdll_il.back());
  EXPECT_EQ(ll_il.front(), stdll_il.front());

  EXPECT_EQ(const_list.back(), const_std_list.back());
  EXPECT_EQ(const_list.front(), const_std_list.front());
  EXPECT_EQ(
      std::is_const_v<std::remove_reference_t<decltype(const_list.back())>>,
      true);
  EXPECT_EQ(
      std::is_const_v<std::remove_reference_t<decltype(const_list.front())>>,
      true);
}

TEST_F(ListTest, EntireListModifiers) {
  dizing::list<testClass> non_empty = {{"dasd", "sad"}, {"dasd", "sad"},
                               {"gdf", "asda"}, {"dsd", "2323"},
                               {"dasd", "sad"}, {"1", "2"}};
  std::list<testClass> std_non_empty = {{"dasd", "sad"}, {"dasd", "sad"},
                                        {"gdf", "asda"}, {"dsd", "2323"},
                                        {"dasd", "sad"}, {"1", "2"}};
  dizing::list<testClass> for_splice = {{"8", "6"}, {"4", "5"}, {"1", "2"},
                                {"0", "0"}, {"0", "0"}, {"0", "0"}};
  std::list<testClass> std_for_splice = {{"8", "6"}, {"4", "5"}, {"1", "2"},
                                         {"0", "0"}, {"0", "0"}, {"0", "0"}};
  check_with_std(non_empty, std_non_empty);
  // SPLICE
  non_empty.splice(non_empty.begin(), for_splice);
  std_non_empty.splice(std_non_empty.begin(), std_for_splice);
  check_with_std(non_empty, std_non_empty);
  check_with_std(for_splice, std_for_splice);
  // UNIQUE
  non_empty.unique();
  std_non_empty.unique();
  check_with_std(non_empty, std_non_empty);
  // REVERSE
  non_empty.reverse();
  std_non_empty.reverse();
  check_with_std(non_empty, std_non_empty);
  // ERASE
  auto res_erase =
      non_empty.erase(++ ++non_empty.begin(), -- --non_empty.end());
  auto res_std_erase =
      std_non_empty.erase(++ ++std_non_empty.begin(), -- --std_non_empty.end());
  check_with_std(non_empty, std_non_empty);
  EXPECT_EQ(*res_erase, *res_std_erase);
  // CLEAR
  non_empty.clear();
  std_non_empty.clear();
  check_with_std(non_empty, std_non_empty);
}

TEST_F(ListTest, Ordering) {
  dizing::list<int> int_list = {1, 3, 3, 5, 7, 11, 1337};
  std::list<int> std_int_list = {1, 3, 3, 5, 7, 11, 1337};
  dizing::list<int> int_list2 = {2, 4, 8, 16, 32, 32, 64, 128, 1024};
  std::list<int> std_int_list2 = {2, 4, 8, 16, 32, 32, 64, 128, 1024};
  check_with_std(int_list, std_int_list);
  check_with_std(int_list2, std_int_list2);
  // MERGE
  int_list.merge(int_list2);
  std_int_list.merge(std_int_list2);
  check_with_std(int_list, std_int_list);
  check_with_std(int_list2, std_int_list2);

  dizing::list<int> int_list3 = {32, 23,   24, 214, 23, 4523, 512, 34,  1232, 12, 31,
                         4,  3456, 45, 523, 34, 23,   421, 534, 63,   453};
  std::list<int> std_int_list3 = {32,  23,   24, 214, 23,  4523, 512,
                                  34,  1232, 12, 31,  4,   3456, 45,
                                  523, 34,   23, 421, 534, 63,   453};
  // SORT
  int_list3.sort();
  std_int_list3.sort();
  check_with_std(int_list3, std_int_list3);
}