#include <gtest/gtest.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <vector>

class Foo {
public:
  auto member_function() {
    auto a = 0;
    auto b = 1.0f;

    // Capture all variables by copy
    auto lambda_0 = [=]() { std::cout << a << b << m_; };

    // Capture all variables by reference
    auto lambda_1 = [&]() { std::cout << a << b << m_; };

    // Capture member variables by reference

    auto lambda_2 = [this]() { std::cout << m_; };

    // Capture member variables by copy
    auto lambda_3 = [*this]() { std::cout << m_; };
  }

private:
  int m_{};
};

auto count_value_above(const std::vector<int>& vals, int th) {
  auto is_above = [th](int v) { return v > th; };
  return std::count_if(vals.begin(), vals.end(), is_above);
}

TEST(LambdaCapture, CaptureByValue) {
  const auto vals = {1, 2, 3, 4, 5, 6};

  auto th = 3;
  auto is_above=[th](int v) { return v > th; };
  th = 4;
  auto count_b = std::count_if(vals.begin(), vals.end(), is_above);

  ASSERT_EQ(3, count_b);
}

TEST(LambdaCapture, CaptureByReference) {
  auto vals = {1, 2, 3, 4, 5, 6};

  auto th = 3;
  auto is_above=[&th](int v) { return v > th; };
  th = 4;
  auto count_b = std::count_if(vals.begin(), vals.end(), is_above);

  ASSERT_EQ(2, count_b);
}

TEST(LambdaCapture, Initialization) {
  auto func = [c = std::list<int>{4,2}] () {
    for(auto v : c) {
      std::cout << v;
    }
  };
  func();
}

TEST(LambdaCapture, MutatingCaptureByValueVSReference) {
  // Capture by value
  {
    auto v = 7;
    auto lambda = [v]() mutable {
      std::cout << v << " ";
      ++v;
    };
    ASSERT_EQ(7, v);
    lambda();
    lambda();
    ASSERT_EQ(7, v);
    std::cout << v;
  }

  // Capture by reference
  {
    auto v = 7;
    auto lambda = [&v]() {
      std::cout << v << " ";
      ++v;
    };
    ASSERT_EQ(7, v);
    lambda();
    lambda();
    ASSERT_EQ(9, v);
    std::cout << v;
  }
}
