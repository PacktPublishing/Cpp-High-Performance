#include <iostream>
#include <gtest/gtest.h>

auto func2() {
  auto i = 0;
  std::cout << "func2(): " << std::addressof(i) << '\n';
}
auto func1() {
  auto i = 0;
  std::cout << "fun1(): " << std::addressof(i) << '\n';
  func2();
}

TEST(StackMemory, StackFrameAddress) {
  auto i = 0;
  std::cout << "main(): " << std::addressof(i) << '\n';
  func1();
  func2();
}
