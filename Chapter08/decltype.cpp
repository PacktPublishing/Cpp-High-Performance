#include <cassert>
#include <list>
#include <vector>
#include <gtest/gtest.h>

template <typename T>
auto square_function(const T& v) {
  return v * v;
}

const auto square_lambda_function = [](const auto& v) {
  return v * v;
};

auto sign_func = [](const auto& v) -> int {
  using ReferenceType = decltype(v);
  using ValueType = std::remove_reference_t<ReferenceType>;
  if (std::is_unsigned_v<ValueType>) {
    return 1;
  }
  return v < 0 ? -1 : 1;
};

template <typename Range>
auto to_vector(const Range& r) {
  using IteratorType = decltype(r.begin());
  using ReferenceType = decltype(*IteratorType());
  using ValueType = std::decay_t<ReferenceType>;
  return std::vector<ValueType>{r.begin(), r.end()};
}

TEST(Decltype, IsSame) {
  auto list = {1, 2, 3, 4, 5};
  auto vector = to_vector(list);
  static_assert(std::is_same_v<decltype(vector), std::vector<int>>);
  ASSERT_EQ(vector.size(), list.size());
}
