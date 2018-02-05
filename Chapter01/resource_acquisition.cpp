#include <cassert>
#include <mutex>
#include <gtest/gtest.h>

//
// This example demonstrates resource acqusition in C++.
// The lifetime of the guard object determines when
// the mutex is being acquired and released.
//

auto func(std::mutex& m, int val, bool b) {
  auto&& guard = std::lock_guard<std::mutex>{m}; // The mutex is locked [auto&& is needed in current version of MSVC]
  if (b) {
    // The lock automatically releases the mutex at early exit
    return;
  }
  if (val == 313) {
    // The lock automatically releases if an exception is thrown
    throw std::exception();
  }
  // The lock automatically releases the mutex at function exit
}


TEST(ResourceAcquisition, Mutex) {
  auto&& m = std::mutex{}; // [auto&& is needed in current version of MSVC]
  auto v = int{};
  auto b = bool{};
  func(m, v, b);
}
