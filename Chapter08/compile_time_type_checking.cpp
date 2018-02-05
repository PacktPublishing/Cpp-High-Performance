#include <cassert>
#include <cstdint>
#include <type_traits>
#include <gtest/gtest.h>

class Infant {};
class Parent {};
class Child : public Parent {};

TEST(CompileTimeTypeChecking, ClassesHierarchy) {
  // Value
  static_assert(std::is_same_v<uint8_t, unsigned char>);
  static_assert(std::is_floating_point_v<decltype(0.3f)>);
  static_assert(std::is_base_of_v<Parent, Child>);
  static_assert(!std::is_base_of_v<Parent, Infant>);

  // Types
  using value_type = std::remove_pointer_t<int*>;
  static_assert((std::is_same_v<value_type, int>));

  using ptr_type = std::add_pointer_t<float>;
  static_assert((std::is_same_v<ptr_type, float*>));
}
