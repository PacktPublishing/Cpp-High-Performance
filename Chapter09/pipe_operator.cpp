#include <gtest/gtest.h>
#include <type_traits>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>

template <typename T>
struct ContainsProxy { const T& value_; };

template <typename Range, typename T>
auto operator|(const Range& r, const ContainsProxy<T>& proxy) {
  const auto& v = proxy.value_;
  return std::find(r.begin(), r.end(), v) != r.end();
}

template <typename T>
auto contains(const T& v) { return ContainsProxy<T>{v}; }

TEST(PipeOperator, Numbers) {
  auto numbers = std::vector<int>{1,3,5,7,9};
  auto seven = 7;
  auto proxy = ContainsProxy<decltype(seven)>{seven};
  bool has_seven = numbers | proxy;
  ASSERT_TRUE(has_seven);
}

TEST(PipeOperator, Strings) {
  auto penguins = std::vector<std::string>{"Ping", "Roy", "Silo"};
  bool has_silo = penguins | contains("Silo");
  ASSERT_TRUE(has_silo);
}
