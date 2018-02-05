#include "chapter_11.hpp"
#include <cassert>
#include <vector>

namespace {

template <typename SrcIt, typename DstIt, typename Pred>
auto copy_if(SrcIt first, SrcIt last, DstIt dst, Pred pred) -> DstIt {
  for (auto it = first; it != last; ++it) {
    if (pred(*it)) {
      *dst = *it;
      ++dst;
    }
  }
  return dst;
}

} // namespace

TEST(CopyIf, OddNumbers) {
  auto numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  auto odd_numbers = std::vector<int>(numbers.size(), -1);
  auto is_odd = [](int v) { return (v % 2) == 1; };

  auto new_end = ::copy_if(numbers.begin(), numbers.end(), odd_numbers.begin(), is_odd);
  odd_numbers.erase(new_end, odd_numbers.end());

  ASSERT_EQ(
    odd_numbers,
    (std::vector<int>{1, 3, 5, 7, 9, 11, 13, 15})
  );

}
