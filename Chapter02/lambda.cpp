#include <gtest/gtest.h>

TEST(Lambda, BasicSyntaxExample) {
  // Prerequisite
  auto vals = std::vector<int>{1, 3, 2, 5, 4};

  // Look for number three
  auto three = 3;
  auto num_threes = std::count(vals.begin(), vals.end(), three);
  ASSERT_EQ(1, num_threes);

  // Look for a numbers which is larger than three
  auto is_above_3 = [](int v){ return v > 3; };
  auto num_above_3 = std::count_if(vals.begin(), vals.end(), is_above_3);
  ASSERT_EQ(2, num_above_3);
}

TEST(Lambda, BasicSyntaxExample2) {
  // Prerequisite
  auto vals = std::vector<int>{1, 3, 2, 5, 4};

  // Look for number three
  auto num_3 = std::count(vals.begin(), vals.end(), 3);
  ASSERT_EQ(1, num_3);

  // Look for a numbers which is larger than three
  auto num_above_3 = std::count_if(vals.begin(), vals.end(), [](int v) {
    return v > 3;
  });
  ASSERT_EQ(2, num_above_3);
}
