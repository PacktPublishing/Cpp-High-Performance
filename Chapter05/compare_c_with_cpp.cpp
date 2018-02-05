#include <gtest/gtest.h>

TEST(CompareCWithCpp, C) {
  // Construct an array
  int array[3] = {22, 44, 66};

  // Iterate
  int* begin = &array[0];
  int* end = &array[3];
  for(int* ptr = begin; ptr != end; ++ptr) {
    // ...
  }
}

TEST(CompareCWithCpp, Cpp) {
  // Construct a simple Linked List
  struct Element {
    Element* next_;
  };
  Element a, b, c;
  a.next_ = &b;
  b.next_ = &c;
  c.next_ = nullptr;

  // Iterate
  Element* begin = &a;
  Element* end = nullptr;
  for(auto ptr = begin; ptr != nullptr; ptr = ptr->next_) {
    // ...
  }
}
