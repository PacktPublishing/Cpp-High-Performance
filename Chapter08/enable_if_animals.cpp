#include <iostream>
#include <type_traits>
#include <gtest/gtest.h>

class AnimalBase {
public:
  virtual ~AnimalBase() {
  };
};

class Bear : public AnimalBase {
public:
  auto roar() const {
    std::cout << "roar" << '\n';
  }
};

class Duck : public AnimalBase {
public:
  auto quack() const {
    std::cout << "quack" << '\n';
  }
};

template <typename Animal>
auto speak(const Animal& a) -> std::enable_if_t<std::is_same_v<Animal, Bear>, void> {
  a.roar();
}

template <typename Animal>
auto speak(const Animal& a) -> std::enable_if_t<std::is_same_v<Animal, Duck>, void> {
  a.quack();
}

TEST(EnableIfAnimals, Speak) {
  Bear bear;
  Duck duck;

  speak(bear);
  speak(duck);
}
