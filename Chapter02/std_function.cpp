#include <gtest/gtest.h>
#include <functional>
#include <vector>
#include <cassert>
#include <iostream>

class Button {
public:
  Button(std::function<void(void)> click)
    : on_click_{click}
  {
  }

  auto on_click() const {
    on_click_();
  }

private:
  std::function<void(void)> on_click_{};
};

auto make_buttons() {
  auto beep_button = Button([beep_count = 0]() mutable {
    std::cout << "Beep:" << beep_count << "! ";
    ++beep_count;
  });
  auto bop_button = Button([]{ std::cout << "Bop. "; });
  auto silent_button = Button([]{});
  auto buttons = std::vector<Button>{
    beep_button,
    bop_button,
    silent_button
  };
  return buttons;
}

TEST(StdFunction, FunctionCall) {
  // Create an unassigned std::function object
  auto func = std::function<void(int)>{};
  // Assign a lambda without capture to the std::function object
  func = [](int v) { std::cout << v; };
  // Execute the lambda
  func(12); // Prints 12

  // Assign a lambda with capture to the same std::function object
  const auto forty_two = 42;
  func = [forty_two](int v) { std::cout << (v + forty_two); };
  func(12); // Prints 54
}

TEST(StdFunction, Buttons) {
  auto buttons = make_buttons();
  for(const auto& button: buttons) {
    button.on_click();
  }
  buttons.front().on_click();
  // Output "Beep: 0! Bop. Beep: 1!"
}
