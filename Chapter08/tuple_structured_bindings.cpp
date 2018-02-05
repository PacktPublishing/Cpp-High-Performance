#include <iostream>
#include <string>
#include <tuple>
#include <gtest/gtest.h>

namespace {

auto make_bond() {
  struct Agent {
    std::string name_{};
    int id_{};
    bool has_license_to_kill_{};
  };
  return Agent{"James", 7, true};
}

} // namespace

TEST(TupleStructuredBindings, Agents) {
  {
    auto agent = make_bond();
    ASSERT_EQ("James", agent.name_);
    ASSERT_EQ(7, agent.id_);
    ASSERT_EQ(true, agent.has_license_to_kill_);
  }
  {
    auto agents = {
            std::make_tuple(std::string{"James"}, 7, true),
            std::make_tuple(std::string{"Nikita"}, 108, false)
    };

    for (auto&&[name, id, has_kill_license] : agents) {
      if (name == "James") {
        ASSERT_EQ("James", name);
        ASSERT_EQ(7, id);
        ASSERT_EQ(true, has_kill_license);
      }
      if (name == "Nikita") {
        ASSERT_EQ("Nikita", name);
        ASSERT_EQ(108, id);
        ASSERT_EQ(false, has_kill_license);
      }
    }
  }
}
