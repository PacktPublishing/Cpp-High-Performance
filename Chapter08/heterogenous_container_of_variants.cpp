#include <algorithm>
#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <gtest/gtest.h>

using VariantType = std::variant<int, std::string, bool>;

TEST(HeterogenousContainerOfVariants, Modification) {
  auto container = std::vector<VariantType>{};
  container.push_back(false);
  container.push_back("I am a string");
  container.push_back("I am also a string");
  container.push_back(13);

  container.pop_back();
  std::reverse(container.begin(), container.end());
}

TEST(HeterogenousContainerOfVariants, AccessingValue) {
  auto container = std::vector<VariantType>{42, std::string{"bingo"}, true};

  for (const auto& val : container) {
    std::visit([](const auto& v){ std::cout << v << '\n'; }, val);
  }

  auto num_bools = std::count_if(container.begin(), container.end(),
    [](const auto& v) {
      return std::holds_alternative<bool>(v);
    }
  );

  auto contains_bingo_string = std::any_of(container.begin(), container.end(),
    [](const auto& v) {
      return
        std::holds_alternative<std::string>(v) &&
        std::get<std::string>(v) == "bingo";
    }
  );
}
