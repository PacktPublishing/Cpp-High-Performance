#if _MSC_VER
  #pragma warning(disable:4244)
#endif
#include <cmath>
#include <type_traits>
#include <gtest/gtest.h>

template <typename T>
auto generic_mod(const T& v, const T& n) -> T {
  if (std::is_floating_point_v<T>) {
    return std::fmod(v, n);
  }
  else {
    return v % n; // if T is a floating point, this line wont compile
  }
}

template <typename T>
auto constexpr_if_generic_mod(const T& v, const T& n) -> T {
  if constexpr (std::is_floating_point_v<T>) {
    return std::fmod(v, n);
  }
  else {
    return v % n; // If T is a floating point, this line is completely removed
  }
}

TEST(ConstexprIfMod, ModOfInts) {
  auto const value = int{7};
  auto const modulus = int{5};
  auto const result = generic_mod(value, modulus);
  auto const constexpr_if_result = constexpr_if_generic_mod(value, modulus);
  ASSERT_EQ(constexpr_if_result, result);
}

TEST(ConstexprIfMod, ModOfFloats) {
  auto const value = float{1.5f};
  auto const modulus = float{1.0f};

  // Compilation error % operation for float value
  // auto const result = generic_mod(value, modulus);
  auto const constexpr_if_result = constexpr_if_generic_mod(value, modulus);
  ASSERT_FLOAT_EQ(0.5f, constexpr_if_result);
}
