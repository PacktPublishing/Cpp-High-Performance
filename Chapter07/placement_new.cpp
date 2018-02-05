#include <iostream>
#include <memory>
#include <gtest/gtest.h>

struct User {
  User(const std::string& name) : name_(name) { }
  std::string name_;
};

TEST(PlacementNew, MallocAllocate) {
  auto memory = std::malloc(sizeof(User));
  auto user = new (memory) User("john");

  ASSERT_EQ("john", user->name_);

  user->~User();
  std::free(memory);
}
