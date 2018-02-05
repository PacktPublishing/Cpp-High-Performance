#include <cassert>
#include <cstdint>
#include <type_traits>
#include <gtest/gtest.h>

template <typename T>
auto sign_func(const T& v) -> int {
  if (std::is_unsigned_v<T>) {
    return 1;
  }
  return v < 0 ? -1 : 1;
}

TEST(TypeTraits, SignFunction) {
  {
    auto unsigned_value = uint32_t{32};
    auto sign = sign_func(unsigned_value);
    ASSERT_EQ(1, sign);
  }
  {
    auto signed_value = int32_t{-32};
    auto sign = sign_func(signed_value);
    ASSERT_EQ(-1, sign);
  }
}
