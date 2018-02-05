#include "chapter_11.hpp"
#include <algorithm>
#include <numeric>

#if PARALLEL_STL_ENABLED == 1
#include <execution>
#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

template <typename T>
auto get_step_size(T start, T stop, size_t num_values) {
  assert(num_values >= 2);
  return (stop - start) / (num_values - 1);
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

  LinearRangeIterator(){}
  LinearRangeIterator(T start, T step_size, size_t value_idx)
  : start_{start}
  , step_size_{step_size}
  , value_idx_{value_idx} {
  }
  auto operator==(const LinearRangeIterator& lri) const {
    return value_idx_ == lri.value_idx_;
  }
  auto operator!=(const LinearRangeIterator& lri) const {
    return !(*this == lri);
  }
  auto& operator++() {
    ++value_idx_;
    return *this;
  }
  auto& operator--() {
    --value_idx_;
    return *this;
  }
  auto operator*() const {
    return start_ + (value_idx_ * step_size_);
  }
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
  auto end() const { return iterator{start_, step_size_, num_values_};}
private:
  T start_{};
  T step_size_{};
  size_t num_values_{};
};

template <typename T>
auto make_linear_range(T start, T stop, size_t num_values) {
  return LinearRange<T> { start, stop, num_values };
}

template <typename Policy, typename IndexType, typename Func>
auto parallel_for(Policy ipolicy, IndexType first, IndexType last, Func func) {
  auto range = make_linear_range<IndexType>(first, last, last);
  std::for_each(std::move(ipolicy), range.begin(), range.end(), std::move(func));
}

} // namespace


TEST(IndexBasedLoop, RangeBasedLoopTypes) {
  auto mice = std::vector<std::string>{"Mickey", "Minnie", "Jerry", "Speedy"};

  for (const auto& m : mice) {
    std::cout << m << '\n';
  }

  std::for_each(mice.begin(), mice.end(), [](const auto& m) {
    std::cout << m << '\n';
  });

  for (size_t i = 0; i < mice.size(); ++i) {
    std::cout << i << " " << mice[i] << '\n';
  }
}

TEST(IndexBasedLoop, LinearRange) {
  {
    auto mice = std::vector<std::string>{"Mickey", "Minnie", "Jerry", "Speedy"};
    auto first_idx = size_t{0};
    auto last_idx = mice.size();
    auto indices = make_linear_range(first_idx, last_idx, last_idx);
    std::for_each(indices.begin(), indices.end(), [&mice](const size_t idx) {
      std::cout << idx << " " << mice[idx] << '\n';
    });
  }

  {
    auto mice = std::vector<std::string>{ "Mickey", "Minnie", "Jerry", "Speedy" };
    auto first_idx = size_t{ 0 };
    auto last_idx = mice.size();
    auto indices = make_linear_range(first_idx, last_idx, last_idx);
    std::for_each(std::execution::par, indices.begin(), indices.end(), [&mice](size_t idx) {
      if (idx == 0) mice[idx] += " is first.";
      else if (idx + 1 == mice.size()) mice[idx] += " is last.";
    });
  }

  {
    auto mice = std::vector<std::string>{ "Mickey", "Minnie", "Jerry", "Speedy" };
    parallel_for(std::execution::par, size_t{0}, mice.size(), [&mice](size_t idx) {
      if (idx == 0) mice[idx] += " is first.";
      else if (idx + 1 == mice.size()) mice[idx] += " is last.";
    });
  }
}

#endif
