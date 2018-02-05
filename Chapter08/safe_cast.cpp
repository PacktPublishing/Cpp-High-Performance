#include <cassert>
#include <cmath>
#include <cstdint>
#include <type_traits>
#include <type_traits> // For std::is_same, std::is_floating_point, std::is_arithmetic etc
#include <cstdint> // For std::uintptr_t and std::intptr_t
#include <cmath> // For std::isnan, std::isinf
#include <gtest/gtest.h>

template <typename T>
constexpr auto make_false() { return false; }

template <typename OutType, typename InType>
auto safe_cast(const InType& v) -> OutType {
  constexpr auto is_same_type = std::is_same_v<InType, OutType>;
  constexpr auto is_pointer_to_pointer =
    std::is_pointer_v<InType> && std::is_pointer_v<OutType>;
  constexpr auto is_float_to_float =
    std::is_floating_point_v<InType> && std::is_floating_point_v<OutType>;
  constexpr auto is_number_to_number =
    std::is_arithmetic_v<InType> && std::is_arithmetic_v<OutType>;
  constexpr auto is_intptr_to_ptr =
    (std::is_same_v<std::uintptr_t, InType> || std::is_same_v<std::intptr_t, InType>) &&
    std::is_pointer_v<OutType>;
  constexpr auto is_ptr_to_intptr =
    std::is_pointer_v<InType> &&
    (std::is_same_v<std::uintptr_t, OutType> || std::is_same_v<std::intptr_t, OutType>);
  // Same type
  if constexpr(is_same_type) {
    return v;
  }
  // intptr to pointer, and vice versa
  else if constexpr(is_intptr_to_ptr || is_ptr_to_intptr) {
    return reinterpret_cast<OutType>(v);
  }
  // Pointer to pointer
  else if constexpr(is_pointer_to_pointer) {
    assert(dynamic_cast<OutType>(v) != nullptr);
    return static_cast<OutType>(v);
  }
  // Double to float, and vice versa
  else if constexpr (is_float_to_float) {
    auto casted = static_cast<OutType>(v);
    auto casted_back = static_cast<InType>(v);
    assert(!isnan(casted_back) && !isinf(casted_back));
    return casted;
  }
  // Number to number
  else if constexpr (is_number_to_number) {
    auto casted = static_cast<OutType>(v);
    auto casted_back = static_cast<InType>(casted);
    assert(casted == casted_back);
    return casted;
  }
  else {
  // static_assert(false, ""); // Enabling this line would make the whole function not compile
    static_assert(make_false<InType>(), "Cast error");
    return OutType{}; // This shall never happen, the static_assert should have failed
  }
}

TEST(SageCast, FloatToInt) {
  auto x = safe_cast<int>(42.0f);
  //auto y = safe_cast<int*>(int16_t{42}); // Does not compile (unless you are using a 16 bit computer :)
}
