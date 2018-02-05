#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <experimental/is_detected_impl.hpp>
#include <gtest/gtest.h>

template <size_t Index, typename TupleType, typename Functor>
auto tuple_at(const TupleType& tpl, const Functor& func) -> void {
  const auto& val = std::get<Index>(tpl);
  func(val);
}

template <typename TupleType, typename Functor, size_t Index = 0>
auto tuple_for_each(const TupleType& tpl, const Functor& ifunctor) -> void {
  constexpr auto tuple_size = std::tuple_size_v<TupleType>;
  if constexpr(Index < tuple_size) {
    tuple_at<Index>(tpl, ifunctor);
    tuple_for_each<TupleType, Functor, Index + 1>(tpl, ifunctor);
  }
}

template <typename T>
using has_reflect_member = decltype(&T::reflect);

template <typename T>
constexpr bool is_reflectable_v = std::experimental::is_detected<has_reflect_member, T>::value;

// Global equal check for reflectable types
template <typename T, bool IsReflectable = is_reflectable_v<T>>
auto operator==(const T& a, const T& b) -> std::enable_if_t<IsReflectable, bool> {
  return a.reflect() == b.reflect();
}

// Global non-equal check for reflectable types
template <typename T, bool IsReflectable = is_reflectable_v<T>>
auto operator!=(const T& a, const T& b) -> std::enable_if_t<IsReflectable, bool> {
  return a.reflect() != b.reflect();
}

// Global less than check for reflectable types
template <typename T, bool IsReflectable = is_reflectable_v<T>>
auto operator<(const T& a, const T& b) -> std::enable_if_t<IsReflectable, bool> {
  return a.reflect() < b.reflect();
}

// Global ostream output for reflectable types
template <typename T, bool IsReflectable = is_reflectable_v<T>>
auto operator<<(std::ostream& ostr, const T& v) -> std::enable_if_t<IsReflectable, std::ostream&> {
  tuple_for_each(v.reflect(), [&ostr](const auto& m) {
      ostr << m << " ";
    });
  return ostr;
}

class Town {
public:
  Town(size_t houses, size_t settlers, const std::string& name)
  : houses_{houses}
  , settlers_{settlers}
  , name_{name} {
  }
  auto reflect() const {
    return std::tie(houses_, settlers_, name_);
  }
private:
  size_t houses_{};
  size_t settlers_{};
  std::string name_{};
};

TEST(Reflection, Towns) {
  auto shire = Town{100, 200, "Shire"};
  auto mordor = Town{1000, 2000, "Mordor"};
  std::cout << shire << '\n'; // Prints "100 200 Shire"
  std::cout << mordor << '\n'; // Prints "1000 2000 Mordor"
  std::cout << (shire < mordor) << '\n'; // Prints "1"
  ASSERT_NE(mordor, shire);
}
