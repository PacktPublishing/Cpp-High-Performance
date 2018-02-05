#if _MSC_VER
  #pragma warning(disable:4244)
#endif
#include <thread>
#include <future>
#include <iostream>
#include <gtest/gtest.h>

namespace {

int divide(int a, int b) {
  if (b == 0) {
    throw std::runtime_error("Divide by zero exception");
  }
  return a / b;
}

} // namespace

TEST(Tasks, Divide) {
  std::packaged_task<decltype(divide)> task(divide);
  auto f = task.get_future();
  std::thread t(std::move(task), 45, 5);
  auto result = int{0};
  ASSERT_NO_THROW({
    result = f.get();
  });
  ASSERT_EQ(45/5, result);
  t.join();
}
