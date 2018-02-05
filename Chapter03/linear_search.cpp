#include <cassert>
#include <iostream>
#include <vector>
#include <gtest/gtest.h>

auto linear_search(const std::vector<int>& a, int key) {
  for (const auto& value : a) {
    if (value == key) {
      return true;
    }
  }
  return false;
}

TEST(LinearSearch, FindNumber) {
  auto a = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8};
  auto found = linear_search(a, 7);

  ASSERT_TRUE(found);
}
