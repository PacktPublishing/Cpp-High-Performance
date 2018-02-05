#include "chapter_11.hpp"
#include <cassert>
#include <atomic>
#include <future>
#include <vector>

namespace {

template <typename SrcIt, typename DstIt, typename Pred>
auto _inner_par_copy_if_sync(
  SrcIt first, 
  SrcIt last, 
  DstIt dst, 
  std::atomic_size_t& dst_idx,
  Pred pred, 
  size_t chunk_sz
) {
  auto n = static_cast<size_t>(std::distance(first, last));
  if (n <= chunk_sz) {
    std::for_each(first, last, [&](const auto& ival) {
      if (pred(ival)) {
        const auto write_idx = dst_idx.fetch_add(1);
        *std::next(dst, write_idx) = ival;
      }
    });
    return;
  }
  auto middle = std::next(first, n / 2);
  auto future = std::async([first, middle, dst, chunk_sz, &pred, &dst_idx] {
    return _inner_par_copy_if_sync(
      first, middle, dst, dst_idx, pred, chunk_sz
    );
  });
  _inner_par_copy_if_sync(middle, last, dst, dst_idx, pred, chunk_sz);
  future.wait();
}

template <typename SrcIt, typename DstIt, typename Pred>
auto par_copy_if_sync(SrcIt first, SrcIt last, DstIt dst, Pred pred, size_t chunk_sz) {
  auto&& dst_write_idx = std::atomic_size_t{0};
  _inner_par_copy_if_sync(first, last, dst, dst_write_idx, pred, chunk_sz);
  return std::next(dst, dst_write_idx);
}

} // namespace

TEST(CopyIfSyncronizedWritePosition, OddNumbers) {
  auto numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  auto odd_numbers = std::vector<int>(numbers.size(), -1);
  auto is_odd = [](int v) { return (v % 2) == 1; };

  auto end = par_copy_if_sync(numbers.begin(), numbers.end(), odd_numbers.begin(), is_odd, 4);
  odd_numbers.erase(end, odd_numbers.end());
  ASSERT_TRUE(std::all_of(odd_numbers.begin(), odd_numbers.end(), is_odd));

  std::sort(odd_numbers.begin(), odd_numbers.end());
  ASSERT_EQ(
    odd_numbers,
    (std::vector<int>{1, 3, 5, 7, 9, 11, 13, 15})
  );

}
