#include <type_traits>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <gtest/gtest.h>

template <typename T>
auto get_step_size(T start, T stop, size_t num_values) {
  assert(num_values >= 2);
  return (stop - start) / (num_values-1);
}

template <typename T>
auto get_linear_value(T start, T step_size, size_t value_idx) {
  return start + step_size * value_idx;
}

template <typename T>
class LinearRangeIterator {
public:
  using difference_type = size_t;
  using value_type = T;
  using reference = T;
  using pointer = void;
  using iterator_category = std::bidirectional_iterator_tag;

  LinearRangeIterator(T start, T step_size, size_t value_idx)
  : start_{start}
  , step_size_{step_size}
  , value_idx_{value_idx}
  {}
  auto operator==(const LinearRangeIterator& lri) const { return value_idx_ == lri.value_idx_; }
  auto operator!=(const LinearRangeIterator& lri) const { return !(*this == lri); }
  auto& operator++() { ++value_idx_; return *this; }
  auto& operator--() { --value_idx_; return *this; }
  auto operator*() const { return start_ + (value_idx_ * step_size_); }
private:
  size_t value_idx_{};
  T start_{};
  T step_size_{};
};

template <typename T>
class LinearRange {
  using iterator = LinearRangeIterator<T>;
public:
  LinearRange(T start, T stop, size_t num_values)
  : start_{start}
  , step_size_{get_step_size(start, stop, num_values)}
  , num_values_{num_values}
  {}
  auto begin() const { return iterator{start_, step_size_, 0}; }
  auto end() const { return iterator{start_, step_size_, num_values_}; }
private:
  T start_{};
  T step_size_{};
  size_t num_values_{};
};

template <typename T>
auto make_linear_range(T start, T stop, size_t num_values) {
  return LinearRange<T>{
    start, stop, num_values
  };
}

TEST(LinearRange, Forward) {
  auto value = 0.0;
  for(auto t: make_linear_range(0.0, 1.0, 4)) {
    std::cout << t << '\n';
    ASSERT_NEAR(value, t, 0.001);
    value += 1.0/3;
  }
}

TEST(LinearRange, Backward) {
  auto value = 1.0;
  for(auto t: make_linear_range(1.0, 0.0, 4)) {
    std::cout << t << '\n';
    ASSERT_NEAR(value, t, 0.001);
    value -= 1.0/3;
  }
}
