#include "chapter_11.hpp"
#include <algorithm>
#if PARALLEL_STL_ENABLED == 1
#include <execution>
#include <iostream>

namespace {

auto find_largest(const std::vector<int>& vals) {
  auto threshold = size_t{2048};
  return vals.size() < threshold ?
    *std::max_element(std::execution::seq, vals.begin(), vals.end()) :
    *std::max_element(std::execution::par, vals.begin(), vals.end());
}

auto inverse_func = [](float denominator) {
  if (denominator == 0.0f) {
    throw std::runtime_error{"Division by zero"};
  }
  return 1.0f / denominator;
};

auto inverse_numbers(const std::vector<float>& numbers, std::vector<float>& oinversed) {
  oinversed.resize(numbers.size(), -1.0f);
  std::transform(
    std::execution::par,
    numbers.begin(),
    numbers.end(),
    oinversed.begin(),
    inverse_func
  );
}

} // namespace

TEST(ParallelStlPolicy, Seq) {
  // seq policy will be used
  auto vals = std::vector<int>{10, 1, 1, 1, 1, 1, 1, 1};
  auto largest = find_largest(vals);
  ASSERT_TRUE(largest == 10);

  // par policy will be used
  vals.resize(2048, 1);
  largest = find_largest(vals);
  ASSERT_TRUE(largest == 10);
}

TEST(ParallelStlPolicy, Par) {
  auto numbers = std::vector<float>{ 3.0f, 4.0f, 0.0f, 8.0f, 2.0f };
  auto inversed = std::vector<float>{};
  try {
    inverse_numbers(numbers, inversed);
  }
  catch (const std::exception& e) {
    std::cout << "Exception thrown, " << e.what() << '\n';
  }
  ASSERT_TRUE(std::count(inversed.begin(), inversed.end(), -1.0f) > 0);
}

#endif
