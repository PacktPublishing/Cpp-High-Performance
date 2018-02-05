#if defined(__cpp_lib_variant) || _MSC_VER >= 1912

#include <iostream>
#include <string>
#include <variant>
#include <gtest/gtest.h>

using VariantType = std::variant<int, std::string, bool>;

struct FunctorImpl {
  auto operator()(const int& v) {
    std::cout << v << '\n';
  }
  auto operator()(const std::string& v) {
    std::cout << v << '\n';
  }
  auto operator()(const bool& v) {
    std::cout << v << '\n';
  }
};

auto visit_impl(FunctorImpl ifunctor, const VariantType& ivariant) {
  if (std::holds_alternative<int>(ivariant)) {
    return ifunctor(std::get<int>(ivariant));
  }
  else if (std::holds_alternative<std::string>(ivariant)) {
    return ifunctor(std::get<std::string>(ivariant));
  }
  else if (std::holds_alternative<bool>(ivariant)) {
    return ifunctor(std::get<bool>(ivariant));
  }
}

TEST(Variant, Assignment) {
  auto v = VariantType{}; // The variant is empty
  v = 7; // v now holds an int
  v = std::string{"rutger"}; // v now holds s std::string, the integer is overwritten
  v = false; // v now holds a bool, the std::string is overwritten
}

TEST(Variant, StdVisit) {
  auto v = VariantType{};
  v = std::string{"visitor"};
  std::visit([](const auto& ival) {
    std::cout << ival << "\n";
  }, v);
}

TEST(Variant, SustomVisitor) {
  auto v = VariantType{};
  v = std::string{"custom visitor"};
  visit_impl(FunctorImpl(), v);
}

#endif