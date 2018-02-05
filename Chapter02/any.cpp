#if defined(__cpp_lib_any) || _MSC_VER >= 1912

#include <gtest/gtest.h>
#include <any>
#include <string>
#include <cassert>

TEST(StdAny, ChangeType) {
  // Initialize a std::any
  auto a = std::any{};

  // Put a string in it
  a = std::string{"something"};

  // Return a reference to the withheld string
  const auto& str_ref = std::any_cast<std::string&>(a);
  // Copy the withheld string
  auto str_copy = std::any_cast<std::string>(a);

  // Put a float in it and read it back
  a = 135.246f;
  const auto& flt = std::any_cast<float>(a);
  ASSERT_TRUE(flt == 135.246f);

  // Copy
  auto b = a;
  auto is_same = a.type() == b.type() && std::any_cast<float>(a) == std::any_cast<float>(b);
  ASSERT_TRUE(is_same);
}

namespace {
template <typename T>
auto is_withheld_type(const std::any& a) -> bool {
  return typeid(T) == a.type();
}
}

TEST(StdAny, IsWithHeldType) {
  auto a = std::any{32.0};
  auto is_int = is_withheld_type<int>(a);
  ASSERT_FALSE(is_int);

  auto is_double = is_withheld_type<double>(a);
  ASSERT_TRUE(is_double);

}

#endif
