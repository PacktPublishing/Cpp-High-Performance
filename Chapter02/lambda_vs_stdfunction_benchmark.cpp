#include <gtest/gtest.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include <chrono>
#include <functional>

TEST(LambdaBenchmark, DirectLambda) {
  auto lbd = [](int v) {
    return v * 3;
  };
  using L = decltype(lbd);
  auto fs = std::vector<L>{};
  fs.resize(1'000'000, lbd);
  auto start = std::chrono::steady_clock::now();
  auto res = int{ 1 };
  for (const auto& f : fs) {
    res = f(res);
  }
  auto stop = std::chrono::steady_clock::now();
  std::cout << "Time invoking vector direct lambdas: " << (stop - start).count() << "\n";
  std::cout << res << "\n";
}

TEST(LambdaBenchmark, StdFunction) {
  auto lbd = [](int v) {
    return v * 3;
  };
  using F = std::function<int(int)>;
  auto fs = std::vector<F>{};
  fs.resize(1'000'000, lbd);
  auto start = std::chrono::steady_clock::now();
  auto res = int{ 1 };
  for (const auto& f : fs) {
    res = f(res);
  }
  auto stop = std::chrono::steady_clock::now();
  std::cout << "Time invoking vector of std::functions: " << (stop - start).count() << "\n";
  std::cout << res << "\n";
}
