#ifdef _MSC_VER
  #define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
  #pragma warning(disable:4996)
#endif
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/transform.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/action/remove_if.hpp>
#include <range/v3/to_container.hpp>

TEST(RangeV3, FlattenedView) {
  namespace rv = ranges::view;
  auto list_of_numberlists = std::vector<std::vector<int>> {
    {1, 2},
    {3, 4, 5},
    {5},
    {1, 2, 3, 4}
  };
  auto flattened_view = rv::join(list_of_numberlists);
  for(const auto& val: flattened_view) {
    std::cout << val << ", ";
  }
  // Prints 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  auto max_value = *std::max_element(flattened_view.begin(), flattened_view.end());
  ASSERT_TRUE(max_value == 5);
}

TEST(RangeV3, OddSquaresView) {
  namespace rv = ranges::view;
  auto numbers = std::vector<int>{1,2,3,4,5,6,7};
  auto odd_squares_view = numbers
    | rv::transform([](auto v){ return v * v; })
    | rv::filter([](auto v){ return (v % 2) == 1; });
  ASSERT_TRUE(
    (odd_squares_view | ranges::to_vector) ==
    (std::vector<int>{1, 9, 25, 49})
  );
}

TEST(RangeV3, StdSortUnique) {
  auto ints = std::vector<int>{ 1, 2, 1, 3, 2, 4 };
  std::sort(ints.begin(), ints.end());
  ASSERT_TRUE(ints == (std::vector<int>{1, 1, 2, 2, 3, 4}));

  auto it = std::unique(ints.begin(), ints.end());
  ASSERT_EQ(
    std::vector<int>(ints.begin(), ints.begin() + 4),
    (std::vector<int>{1, 2, 3, 4})
  );
  // ints starts with "1, 2, 3, 4"
  // it points to the fifth element

  ints.erase(it, ints.end());
  ASSERT_TRUE(ints == (std::vector<int>{1, 2, 3, 4}));
}

TEST(RangeV3, RangesSortUnique) {
  namespace ra = ranges::action;
  auto ints = std::vector<int>{ 1, 2, 1, 3, 2, 4 }
    | ra::sort
    | ra::unique;
  ASSERT_TRUE(ints == (std::vector<int>{1, 2, 3, 4}));
}

TEST(RangeV3, Taransform) {
  namespace rv = ranges::view;
  auto ints = std::vector<int>{2, 4, 6};
  std::vector<std::string> str_vec = ints
    | rv::transform([](auto v){ return std::to_string(v); });

  ASSERT_TRUE(str_vec == (std::vector<std::string>{"2", "4", "6"}));
}

TEST(RangeV3, TransformToVector) {
  namespace rv = ranges::view;
  auto ints = std::vector<int>{2,4,6};
  auto str_vec = ints
    | rv::transform([](auto v){ return std::to_string(v); })
    | ranges::to_vector;

  ASSERT_TRUE(str_vec == (std::vector<std::string>{"2", "4", "6"}));
}

TEST(RangeV3, RemoveIf) {
  namespace ra = ranges::action;
  auto get_vals = []() { return std::vector<int>{1, 3, 5, 7}; };
  auto is_above_5 = [](auto v) { return v >= 5; };

  {
    auto vals = get_vals() | ra::remove_if(is_above_5);
    ASSERT_TRUE(vals == (std::vector<int>{ 1, 3 }));
  }
  {
    auto vals = get_vals();
    vals = std::move(vals) | ra::remove_if(is_above_5);
    ASSERT_TRUE(vals == (std::vector<int>{ 1, 3 }));
  }
  {
    auto vals = get_vals();
    vals |= ra::remove_if(is_above_5);
    ASSERT_TRUE(vals == (std::vector<int>{ 1, 3 }));
  }
}



namespace {
auto get() { return std::vector<int>{1, 2, 3, 4}; }
}

TEST(RangeV3, Actions_VS_Views) {
  // Prerequisite
  auto is_odd = [](int v) { return (v % 2) == 1; };
  auto square = [](int v) { return v * v; };
  namespace ra = ranges::action;
  namespace rv = ranges::view;
  // Print odd squares using actions...
  for (auto v : get() | ra::remove_if(is_odd) | ra::transform(square)) {
    std::cout << v << " ";
  }
  // ... corresponds to the following code where c is mutated:
  auto c = get();
  auto new_end = std::remove_if(c.begin(), c.end(), is_odd);
  c.erase(new_end, c.end());
  std::transform(c.begin(), c.end(), c.begin(), square);
  for (auto v : c) { std::cout << v << " "; }


  // Print odd squares using a view...
  const auto vals = get();
  for (auto v : vals | rv::remove_if(is_odd) | rv::transform(square)) {
    std::cout << v << " ";
  }
  // ... corresponds to the following code where get() is only iterated
  for (auto v : get()) {
    if (is_odd(v)) {
      auto s = square(v);
      std::cout << s << " ";
    }
  }


  const auto result_by_view = vals | rv::remove_if(is_odd) | rv::transform(square) | ranges::to_vector;
  const auto result_by_action = get() | ra::remove_if(is_odd) | ra::transform(square);
  ASSERT_EQ(result_by_view, result_by_action);

}