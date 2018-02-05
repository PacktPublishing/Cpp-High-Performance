#include <type_traits>
#include <gtest/gtest.h>

template <typename T, bool IsFloat = std::is_floating_point_v<T> >
auto interpolate(T left, T right, T power) -> std::enable_if_t<IsFloat, T> {
  return left * (1 - power) + right * power;
}

TEST(EnableIf, IsSame) {
  auto valuef = interpolate(5.0f, 6.0f, 0.5f);
  static_assert(std::is_same_v<decltype(valuef), float>);

  auto valued = interpolate(5.0, 6.0, 0.5);
  static_assert(std::is_same_v<decltype(valued), double>);
}
