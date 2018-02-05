#include <cassert>
#include <iostream>
#include <vector>
#include <gtest/gtest.h>

auto binary_search(const std::vector<int>& a, int key) {
  if (a.empty()) {
    return false;
  }
  auto low = size_t(0);
  auto high = a.size() - 1;
  while (low <= high) {
    const auto mid = low + ((high - low) / 2);
    if (a[mid] < key) {
      low = mid + 1;
    }
    else if (a[mid] > key) {
      high = mid - 1;
    }
    else {
      return true;
    }
  }
  return false;
}

TEST(BinarySearch, FindNumber) {
  auto a = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8};
  auto found = binary_search(a, 7);

  ASSERT_TRUE(found);
}
