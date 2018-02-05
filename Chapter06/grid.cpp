#include <gtest/gtest.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdlib> // For std::rand

template <typename Iterator, typename Value>
auto contains(const Iterator& begin, const Iterator& end, const Value& v) {
  for (auto it = begin; it != end; ++it) {
    if (*it == v) {
      return true;
    }
  }
  return false;
}

class Grid {
public:
  Grid(size_t width, size_t height)
  : width_{width}
  , height_{height}
  {
    data_.resize(width*height);
  }
  auto begin() { return data_.begin(); }
  auto end() { return data_.end(); }
  auto& at(size_t x, size_t y) { return data_[y*width_+x]; }
  auto get_row(size_t y) {
    auto left = begin() + width_ * y;
    auto right = left + width_;
    return std::make_pair(left, right);
  }
private:
  std::vector<int> data_{};
  size_t width_{};
  size_t height_{};
};

TEST(GridIterators, CountElements) {
  auto grid = Grid{ 10, 10 };
  auto y = 3;
  auto r = grid.get_row(y);
  std::generate(r.first, r.second, std::rand);
  auto num_fives = std::count(r.first, r.second, 5);
  std::cout << "Num fives in row " << y << ": " << num_fives << "\n";
}
