#include <iterator>
#include <type_traits>
#include <set>
#include <list>
#include <gtest/gtest.h>

template <typename Iterator>
auto iterator_distance(Iterator a, Iterator b) {
  using Traits = typename std::iterator_traits<Iterator>;
  using Category = typename Traits::iterator_category;
  using Difference = typename Traits::difference_type;
  constexpr auto is_random_access = std::is_same_v<Category, std::random_access_iterator_tag>;
  if constexpr(is_random_access) {
    return b - a;
  }
  else {
    auto steps = Difference{};
    while (a != b) {
      ++steps; ++a;
    }
    return steps;
  }
}

TEST(IteratorDistance, RandomIterator) {
  auto numbers = std::vector<int>{1,2,3,4,5};
  ASSERT_EQ(numbers.size(), iterator_distance(numbers.begin(), numbers.end()));
}

TEST(IteratorDistance, ForwardIterator) {
  auto numbers = std::list<int>{1,2,3,4,5};
  ASSERT_EQ(numbers.size(), iterator_distance(numbers.begin(), numbers.end()));
}
