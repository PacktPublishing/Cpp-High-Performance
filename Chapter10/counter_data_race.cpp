#include <cassert>
#include <iostream>
#include <thread>
#include <gtest/gtest.h>

// This program contains a data race since two thread
// are accessing the global counter variable without synchronization.

namespace {

int counter = 0;

auto increment_counter(int n) {
  for (int i = 0; i < n; i++) {
    ++counter;
  }
}

} // namespace

TEST(CounterDataRace, IncrementCounter) {
  const auto n_times = 1000000;
  std::thread t1(increment_counter, n_times);
  std::thread t2(increment_counter, n_times);

  t1.join();
  t2.join();

  // Should output 2000000 but will probably output something less than 2000000.
  std::cout << counter << '\n';
}
