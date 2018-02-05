#include <cassert>
#include <gtest/gtest.h>

template <typename T>
class Rectangle {
public:
  Rectangle(T x, T y, T w, T h) :
    x_{x}, y_{y}, width_{w}, height_{h} {
  }
  auto area() const {
    return width_ * height_;
  }
  auto width() const {
    return width_;
  }
  auto height() const {
    return height_;
  }

private:
  T x_{};
  T y_{};
  T width_{};
  T height_{};
};

template <typename T>
auto is_square(const Rectangle<T>& r) -> bool {
  return r.width() == r.height();
}

TEST(TemplateClass, FloatSquare) {
  auto rectf = Rectangle<float>{2.0f, 2.0f, 4.0f, 4.0f}; // rectf is a rectangle of floats
  ASSERT_TRUE(is_square(rectf));
}

TEST(TemplateClass, IntSquare) {
  auto recti = Rectangle<int>{-2, -2, 4, 4}; // recti is a rectangle of integers
  ASSERT_TRUE(is_square((recti)));
}
