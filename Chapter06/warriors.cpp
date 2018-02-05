#if _MSC_VER
  #define _SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
#endif

#include <gtest/gtest.h>
#include <vector>
#include <cassert>
#include <iostream>
#include <string>
#include <algorithm>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/algorithm/max_element.hpp>

enum class EAbility { Fencing, Archery };

class Warrior {
public:
  EAbility ability_{};
  int level_{};
  std::string name_{};
};

auto is_archer_func = [](const Warrior& w) -> bool { return w.ability_ == EAbility::Archery; };
auto compare_level_func = [](const Warrior& a, const Warrior& b) -> bool { return a.level_ < b.level_; };

auto get_max_archor_level(const std::vector<Warrior>& warriors) -> int {
  auto archery = std::vector<Warrior>{};
  // The warrior list needs to be copied in order to filter to archery only
  std::copy_if(warriors.begin(), warriors.end(), std::back_inserter(archery), is_archer_func);
  auto max_level_it = std::max_element(archery.begin(), archery.end(), compare_level_func);
  return max_level_it->level_;
}

auto get_max_archor_level_for_loop(const std::vector<Warrior>& warriors) -> int {
  auto max_level = int{0};
  for(const auto& w: warriors) {
    if(w.ability_ == EAbility::Archery) {
      max_level = std::max(max_level, w.level_);
    }
  }
  return max_level;
}

auto get_max_archor_level_range_v3(const std::vector<Warrior>& warriors) -> int {
  auto archer_levels = warriors
    | ranges::view::filter(is_archer_func)
    | ranges::view::transform([](const auto& w) { return w.level_; });
  auto max_level_it = ranges::max_element(archer_levels);
  return *max_level_it;
}

TEST(Warriors, GetMaxArchorLevel) {
  auto warriors = std::vector<Warrior> {
    Warrior{EAbility::Fencing, 12, "Zorro"},
    Warrior{EAbility::Archery, 10, "Legolas"},
    Warrior{EAbility::Archery, 7, "Link"}
  };
  ASSERT_TRUE( get_max_archor_level_for_loop(warriors) == 10 );
  ASSERT_TRUE( get_max_archor_level_range_v3(warriors) == 10 );
}
