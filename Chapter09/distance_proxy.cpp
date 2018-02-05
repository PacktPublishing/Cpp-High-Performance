#include <gtest/gtest.h>
#include <type_traits>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <cmath>

class DistProxy{
public:
  DistProxy(float x0, float y0, float x1, float y1)
  : dist_sqrd_{static_cast<float>(std::pow(x0 - x1, 2) + std::pow(y0 - y1, 2))}
  {}
  auto operator==(const DistProxy& dp) const { return dist_sqrd_ == dp.dist_sqrd_; }
  auto operator<(const DistProxy& dp) const { return dist_sqrd_ < dp.dist_sqrd_; }
  auto operator<(float dist) const { return dist_sqrd_ < std::pow(dist, 2); }
  // Implicit cast to float
  operator float() const && { return std::sqrt(dist_sqrd_); }
private:
  float dist_sqrd_{};
};

class Point {
public:
  Point(float x, float y) : x_{x}, y_{y} {}
  auto distance(const Point& p) const {
    return DistProxy{x_, y_, p.x_, p.y_};
  }
  float x_{};
  float y_{};
};

TEST(DistanceProxy, NearestToBingo) {
  auto bingo = Point{ 31, 11 };
  auto a = Point{ 23, 42 };
  auto b = Point{ 33, 12 };
  // These "DistProxy" proxy objects are never visible from the outside
  DistProxy a_to_bingo = a.distance(bingo);
  DistProxy b_to_bingo = b.distance(bingo);
  // The member operator< on DistProxycis invoked, which compares the member dist_sqrd_;
  auto a_is_nearest = a_to_bingo < b_to_bingo;
}

TEST(DistanceProxy, NearestToBingo2) {
  auto bingo = Point{31, 11};
  auto a = Point{23, 42};
  auto b = Point{33, 12};
  bool a_is_nearest = a.distance(bingo) < b.distance(bingo);
}

TEST(DistanceProxy, ABDistance) {
  auto a = Point{23, 42};
  auto b = Point{33, 12};
  float dist = a.distance(b); // Note that we do not use auto here
}

TEST(DistanceProxy, ABDistance2) {
  auto a = Point{23, 42};
  auto b = Point{33, 12};
  auto dist = a.distance(b);
  static_assert(std::is_same_v<DistProxy, decltype(dist)>, "");
  // float dist_float = dist; // This does not compile because of && modifier on operator float()
}
