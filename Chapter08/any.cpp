#if defined(__cpp_lib_any) || _MSC_VER >= 1912

#include <any>
#include <iostream>
#include <vector>
#include <gtest/gtest.h>

TEST(Any, VectorOfAny) {
  auto container = std::vector<std::any>{42, "hi", true};

  for (const auto& a : container) {
    if (a.type() == typeid(int)) {
      const auto& value = std::any_cast<int>(a);
      ASSERT_EQ(42, value );
    }
    else if (a.type() == typeid(const char*)) {
      const auto& value = std::any_cast<const char*>(a);
      ASSERT_EQ("hi", std::string(value));
    }
    else if (a.type() == typeid(bool)) {
      const auto& value = std::any_cast<bool>(a);
      ASSERT_EQ(true, value);
    }
  }

  // Compilation error
  // for(const auto& a: container) {
  //   std::cout << a;
  // }
}

#endif