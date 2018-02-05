#include <type_traits>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <gtest/gtest.h>

class IntIterator {
public:
  using difference_type = size_t;
  using value_type = int;
  using reference = int&;
  using pointer = int*;
  using iterator_category = std::bidirectional_iterator_tag;

  IntIterator(int value) : value_{value} {}

  // Observers
  auto operator==(const IntIterator& it) const {
    return value_ == it.value_;
  }

  auto operator!=(const IntIterator& it) const {
    return !(*this == it);
  }

  auto operator<(const IntIterator& it) const {
    return value_ < it.value_;
  }

  auto& operator*() const {
    return value_;
  }

  // Mutators
  auto& operator++() {
    ++value_;
    return *this;
  }

  auto& operator--() {
    --value_;
    return *this;
  }

private:
  int value_{};
};

template <typename Iterator>
auto distance(Iterator a, Iterator b) {
  using IteratorCategory = typename std::iterator_traits<Iterator>::iterator_category;
  constexpr auto is_random_access = std::is_same_v<IteratorCategory, std::random_access_iterator_tag>;
  if constexpr(is_random_access) {
    return b - a;
  }
  else {
    auto steps = 0;
    while(a != b) { ++steps; }
    return steps;
  }
}

TEST(Iterator, Copy) {
  auto numbers = std::vector<int>{};
  std::copy(IntIterator(5), IntIterator(12), std::back_inserter(numbers));
  ASSERT_EQ(std::vector<int>({5, 6, 7, 8, 9, 10, 11}), numbers);
}

TEST(Iterator, ForLoop) {
  auto counter = int{12};
  for (auto it = IntIterator(12); it != IntIterator(-1); --it)
  {
    std::cout << *it << '\n';
    ASSERT_EQ(counter, *it);
    counter -= 1;
  }
}
