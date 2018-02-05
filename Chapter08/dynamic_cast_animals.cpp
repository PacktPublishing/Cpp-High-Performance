#include <iostream>
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

auto speak(const AnimalBase* ianimal_ptr) {
  const auto* bear = dynamic_cast<const Bear*>(ianimal_ptr);
  const auto* duck = dynamic_cast<const Duck*>(ianimal_ptr);

  if (bear != nullptr) {
    bear->roar();
  }
  else if (duck != nullptr) {
    duck->quack();
  }
}

TEST(DynamicCastAnimals, Speak) {
  Bear bear;
  Duck duck;

  speak(&bear);
  speak(&duck);
}
