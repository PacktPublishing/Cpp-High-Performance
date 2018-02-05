#include <experimental/is_detected_impl.hpp>
#include <gtest/gtest.h>

struct Octopus {
  auto mess_with_arms() {}
};

struct Whale {
  auto blow_a_fountain() {}
};

template <typename T> using can_mess_with_arms = decltype(&T::mess_with_arms);
template <typename T> using can_blow_a_fountain = decltype(&T::blow_a_fountain);

struct Shark {
  using fin_type = float;
};

struct Eel {
  int electricity_{};
};

template <typename T> using has_fin_type = typename T::fin_type;
template <typename T> using has_electricity = decltype(T::electricity_);

TEST(IsDetected, FishTester) {
  using namespace std::experimental;

  // Octopus
  static_assert(is_detected<can_mess_with_arms, Octopus>::value);
  static_assert(!is_detected<can_blow_a_fountain, Octopus>::value);

  // Whale
  static_assert(!is_detected<can_mess_with_arms, Whale>::value);
  static_assert(is_detected<can_blow_a_fountain, Whale>::value);
}

TEST(IsDetected, SharkANdShrimpTester) {
  using namespace std::experimental;

  // The shark has a fin type but no electricity
  static_assert(is_detected<has_fin_type, Shark>::value);
  static_assert(!is_detected<has_electricity, Shark>::value);

  // The eel has electricity but no fins
  static_assert(is_detected<has_electricity, Eel>::value);
  static_assert(!is_detected<has_fin_type, Eel>::value);
}
