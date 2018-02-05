#include <cassert>
#include <vector>
#include <gtest/gtest.h>


struct Leaf{ /* ... */ };
struct Branch{ /* ... */ };


class OakTree {
public:
  auto& operator=(const OakTree& other) {
    leafs_ = other.leafs_;
     // If this throws, only the leafs has been copied
     // and the Oaktree is set to an invalid state
    branches_ = other.branches_;
    return *this;
  }
  std::vector<Leaf> leafs_;
  std::vector<Branch> branches_;
};




class OakTreeExceptionSafe {
public:
  auto& operator=(const OakTree& other) {
    // Perform operations which may throw without modifying
    // the state of this Oaktree
    auto leafs = other.leafs_; // May throw, but OakTree
    // is still valid
    auto branches = other.branches_; // May throw, but OakTree
    // is still valid
    // No exceptions thrown, we can safely modify
    // the state of this by non-throwing functions
    std::swap(leafs_, leafs);
    std::swap(branches_, branches);
    return *this;
  }
  std::vector<Leaf> leafs_;
  std::vector<Branch> branches_;
};

auto make_big_oak_tree() { return OakTree{std::vector<Leaf>(1000), std::vector<Branch>(100)};}
auto make_small_oak_tree() { return OakTree{std::vector<Leaf>(50), std::vector<Branch>(5)}; }
auto save_to_disk(std::vector<OakTree> trees) {};


TEST(ExceptionSafety, OakTree) {

  auto oaktree0 = make_big_oak_tree();
  auto oaktree1 = make_small_oak_tree();
  try {
      oaktree0 = oaktree1;
  }
  catch(const std::exception& e) {
    (void)(e);
    // Oaktree0 might be broken
    save_to_disk({oaktree0, oaktree1});
  }

}
