#include <iostream>
#include <string>
#include <tuple>
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

template <typename TupleType, typename Functor, size_t Index = 0>
auto tuple_any_of(const TupleType& tpl, const Functor& ifunc) -> bool {
  constexpr auto tuple_size = std::tuple_size_v<TupleType>;
  if constexpr(Index < tuple_size) {
    bool success = ifunc(std::get<Index>(tpl));
    return success ?
      true :
      tuple_any_of<TupleType, Functor, Index + 1>(tpl, ifunc);
  }
  else {
    return false;
  }
}

TEST(TupleAlgorithms, ForEach) {
  auto tpl = std::make_tuple(1, true, std::string("Jedi"));
  tuple_for_each(tpl, [](const auto& ival) {
    std::cout << ival << " ";
  });
}

TEST(TupleAlgorithms, AnyOf) {
  auto tuple = std::make_tuple(42, 43.0f, 44.0);
  auto contains_44 = tuple_any_of(tuple, [](const auto& v) {
    return v == 44;
  });
}
