#include <iostream>
#include <gtest/gtest.h>

// Memory returned from new is correctly aligned for std::max_align_t
// which means that it is also correctly aligned for any scalar type
TEST(Alignment, PointerAddress) {
  auto* p = new char{};
  auto address = reinterpret_cast<std::uintptr_t>(p);
  auto max_alignment = alignof(std::max_align_t);
  std::cout << (address % max_alignment) << '\n'; // Outputs 0

  ASSERT_EQ(0, address % max_alignment);
}
