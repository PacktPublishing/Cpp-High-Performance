#include <gtest/gtest.h>
#include <vector>
#include <cassert>
#include <algorithm>

template <typename It, typename Value>
auto find_slow(It first, It last, const Value& value) {
  for(auto it = first; it != last; ++it)
    if(*it == value)
      return it;
  return last;
}

template <typename It, typename Value>
auto find_fast(It first, It last, const Value& value) {
  // Main loop unrolled into chunks of four
  auto num_trips = (last - first) / 4;
  for (auto trip_count = num_trips; trip_count > 0; --trip_count) {
    if (*first == value) {return first;} ++first;
    if (*first == value) {return first;} ++first;
    if (*first == value) {return first;} ++first;
    if (*first == value) {return first;} ++first;
  }
  // Handle the remaining elements
  switch (last - first) {
    case 3: if (*first == value) {return first;} ++first;
    case 2: if (*first == value) {return first;} ++first;
    case 1: if (*first == value) {return first;} ++first;
    case 0:
    default: return last;
  }
}

TEST(FindAlgorithm, SlowAndFast) {
  auto vals = std::vector<int>{1,3,2,4,3,5,4,6,5,7,6,8,7,9,0};

  auto a = find_slow(vals.begin(), vals.end(), 7);
  ASSERT_TRUE(a != vals.end());

  auto b = find_fast(vals.begin(), vals.end(), 7);
  ASSERT_TRUE(b != vals.end());
}
