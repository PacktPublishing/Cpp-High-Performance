#include <gtest/gtest.h>
#include <type_traits>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>

template <typename T> struct InProxy { const T& val_; };
struct InTag{};
constexpr static auto in = InTag{};

template <typename T>
auto operator<(const T& v, const InTag&) { return InProxy<T>{v}; }

template <typename T, typename Range>
auto operator>(const InProxy<T>& proxy, const Range& r) {
  return std::find(r.begin(), r.end(), proxy.val_) != r.end();
}

TEST(InfixOperator, SwedenInAfricaProxy) {
  auto africa = std::vector<std::string>{ "Kenya", "Ethiopia", "Kongo", "Gambia" };
  auto sweden = std::string{"Sweden"};
  InTag in_tag;
  InProxy<std::string> proxy = sweden < in_tag;
  bool is_sweden_in_africa = proxy > africa; // is_sweden_in_africa equals false;
  ASSERT_FALSE(is_sweden_in_africa);
}

TEST(InfixOperator, SwedenInAfricaOperator) {
  auto africa = std::vector<std::string>{ "Kenya", "Ethiopia", "Kongo", "Gambia" };
  auto sweden = std::string{"Sweden"};
  auto is_sweden_in_africa = sweden <in> africa;
  ASSERT_FALSE(is_sweden_in_africa);
}
