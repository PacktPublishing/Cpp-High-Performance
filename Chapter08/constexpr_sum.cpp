#include <cassert>
#include <iostream>
#include <type_traits>
#include <gtest/gtest.h>

constexpr auto sum(int ix, int iy, int iz) {
  return ix + iy + iz;
}

TEST(ConstexprSum, Compiletime) {
  // value will be evaluated compile-time
  const auto compile_time_value = sum(3, 4, 5);
  static_assert(compile_time_value == 12);

  // Ensure compile-time value evaluation
  typedef std::integral_constant<int, sum(1, 2, 3)> sum6_t;
  static_assert(sum6_t::value == 6);

  // Compilation error, can't be evaluated compile-time (uncomment to check)
  // std::integral_constant<int, sum(x, y, z)> sumxyz_t;
}

// Run tests with --gtest_also_run_disabled_tests option to invoke this test
TEST(ConstexprSum, DISABLED_Runtime) {
  // value will be evaluated run-time
  int x = 0, y = 0, z = 0;
  std::cout << "Enter x y z values: ";
  std::cin >> x >> y >> z; // Take input from user
  const auto run_time_value = sum(x, y, z);
  ASSERT_EQ(x + y + z, run_time_value);
}
