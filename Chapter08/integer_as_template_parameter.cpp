#include <cassert>
#include <gtest/gtest.h>

template <int N, typename T>
auto const_pow_n(const T& v) {
  static_assert(N >= 0, "N must be positive");
  auto product = T{1};
  for (int i = 0; i < N; ++i)
    product *= v;
  return product;
}

TEST(IntegerAsTemplateParameter, ConstPowN) {
  // The compiler generates a function which squares the value
  auto x = const_pow_n<2, float>(4.0f);
  ASSERT_FLOAT_EQ(16.0f, x);

  // The compiler generates a function which cubes the value
  auto y = const_pow_n<3, float>(4.0f);
  ASSERT_FLOAT_EQ(64.0f, y);
}
