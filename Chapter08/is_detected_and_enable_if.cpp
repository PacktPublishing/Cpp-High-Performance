#include <iostream>
#include <string>
#include <experimental/is_detected_impl.hpp>
#include <gtest/gtest.h>

template <typename T> using has_to_string = decltype(&T::to_string);
template <typename T> using has_name_member = decltype(T::name_);

// Print the to_string() function if it exists in class
template <
  typename T,
  bool HasToString = std::experimental::is_detected<has_to_string, T>::value,
  bool HasNameMember = std::experimental::is_detected<has_name_member, T>::value
>
auto print(const T& v) -> std::enable_if_t<HasToString && !HasNameMember> {
  std::cout << v.to_string() << '\n';
}

// Print the name_ member variable if it exists in class
template <
  typename T,
  bool HasToString = std::experimental::is_detected<has_to_string, T>::value,
  bool HasNameMember = std::experimental::is_detected<has_name_member, T>::value
>
auto print(const T& v) -> std::enable_if_t<HasNameMember && !HasToString> {
  std::cout << v.name_ << '\n';
}

struct Squid {
  auto to_string() const {
    return std::string{"I am a little squid"};
  }
};

struct Salmon {
  Salmon() : name_{"Jeff the Salmon"} {}
  std::string name_{};
};

TEST(IsDetectedAndEnableIf, NameAndToString) {
  print(Squid{}); // Prints "I am a little squid"
  print(Salmon{}); // Prints "Jeff the Salmon"
}
