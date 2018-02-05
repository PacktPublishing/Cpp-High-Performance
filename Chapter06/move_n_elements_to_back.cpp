#include <gtest/gtest.h>
#include <vector>
#include <array>
#include <algorithm>
#include <cassert>

namespace {

template <typename Container>
auto move_n_elements_to_back_invalid_a(Container& c, size_t n) {
  // Copy the first n elements to the end of the container
  for(auto it = c.begin(); it != std::next(c.begin(), n); ++it) {
    c.emplace_back(std::move(*it));
  }
  // Erase the copied elements from front of container
  c.erase(c.begin(), std::next(c.begin(), n));
}

template <typename Container>
auto move_n_elements_to_back_invalid_b(Container& c, size_t n) {
  for(size_t i = 0; i < n; ++i) {
    auto&& value = *std::next(c.begin(), i);
    c.emplace_back(std::move(value));
  }
  c.erase(c.begin(), std::next(c.begin(), n));
}

template <typename Container>
auto move_n_elements_to_back(Container& c, size_t n) {
  auto new_begin = std::next(c.begin(), n);
  std::rotate(c.begin(), new_begin, c.end());
}


}


TEST(MoveNElementsToBack, StdVector) {
  {
    auto vec = std::vector<int>{ 1,2,3,4,5,6,7,8,9 };
    // move_n_elements_to_back_invalid_a(vec, 6); // Crashes
    move_n_elements_to_back_invalid_b(vec, 6); // Slow
    ASSERT_TRUE(vec == (std::vector<int>{7, 8, 9, 1, 2, 3, 4, 5, 6}));
  }
  {
    auto vec = std::vector<int>{1,2,3,4,5,6,7,8,9};
    // move_n_elements_to_back_invalid_a(vec, 6); // Crashes
    move_n_elements_to_back(vec, 6); // As good as it gets
    ASSERT_TRUE(vec == (std::vector<int>{7,8,9,1,2,3,4,5,6}));
  }
}


TEST(MoveNElementsToBack, StdArray) {
    auto arr = std::array<int, 9>{1,2,3,4,5,6,7,8,9};
    // move_n_elements_to_back_invalid_a(arr, 6); // Does not compile
    // move_n_elements_to_back_invalid_b(arr, 6); // Does not compile
    move_n_elements_to_back(arr, 6); // As good as it gets
    ASSERT_TRUE(arr == (std::array<int, 9>{7,8,9,1,2,3,4,5,6}));
}
