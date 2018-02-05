#include <iostream>
#include <string>
#include <tuple>
#include <gtest/gtest.h>

namespace {

auto make_bond() {
  return std::make_tuple(std::string{"James"}, 7, true);
}

} // namespace

TEST(TupleGetTieStructuredCoparison, StdGet) {
  // Using std::get<N>
  auto tpl = make_bond();
  auto name = std::get<0>(tpl);
  auto id = std::get<1>(tpl);
  auto has_kill_license = std::get<2>(tpl);

  ASSERT_EQ("James", name);
  ASSERT_EQ(7, id);
  ASSERT_EQ(true, has_kill_license);
}

TEST(TupleGetTieStructuredCoparison, StdTie) {
  // Using std::tie
  auto name = std::string{};
  auto id = int{};
  auto has_kill_license = bool{};
  std::tie(name, id, has_kill_license) = make_bond();

  ASSERT_EQ("James", name);
  ASSERT_EQ(7, id);
  ASSERT_EQ(true, has_kill_license);
}

TEST(TupleGetTieStructuredCoparison, StructuredBinding) {
  auto&&[name, id, has_kill_license] = make_bond();

  ASSERT_EQ("James", name);
  ASSERT_EQ(7, id);
  ASSERT_EQ(true, has_kill_license);
}

TEST(TupleGetTieStructuredCoparison, ForLoopWithStructuredBinding) {
  auto agents = {
        std::make_tuple(std::string{"James"}, 7, true),
        std::make_tuple(std::string{"Nikita"}, 108, false)
  };

  for (auto&&[name, id, has_kill_license] : agents) {
    if ("James" == name) {
      ASSERT_EQ(7, id);
      ASSERT_EQ(true, has_kill_license);
    }
    if ("Nikita" == name) {
      ASSERT_EQ(108, id);
      ASSERT_EQ(false, has_kill_license);
    }
  }
}
