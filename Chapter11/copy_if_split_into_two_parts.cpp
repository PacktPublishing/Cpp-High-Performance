#include "chapter_11.hpp"
#include <cassert>
#include <atomic>
#include <future>
#include <vector>

namespace {

template <typename SrcIt, typename DstIt, typename Pred>
auto par_copy_if_split(SrcIt first, SrcIt last, DstIt dst, Pred pred, size_t chunk_sz) -> DstIt {
  auto n = static_cast<size_t>(std::distance(first, last));
  using CopiedRange = std::pair<DstIt, DstIt>;
  using FutureType = std::future< CopiedRange >;
  auto futures = std::vector<FutureType>{};
  futures.reserve(n / chunk_sz);

  for (size_t start_idx = 0; start_idx < n; start_idx += chunk_sz) {
    const auto stop_idx = std::min(start_idx + chunk_sz, n);
    auto future = std::async([=, &pred] {
      auto dst_first = dst + start_idx;
      auto dst_last = std::copy_if(first + start_idx, first + stop_idx,
      dst_first, pred);
      return std::make_pair(dst_first, dst_last);
    });
    futures.emplace_back(std::move(future));
  }

  auto new_end = futures.front().get().second;
  for (auto it = std::next(futures.begin()); it != futures.end(); ++it) {
    auto chunk_range = it->get();
    new_end = std::move(chunk_range.first, chunk_range.second, new_end);
  }
  return new_end;
}

} // namespace

TEST(CopyIfSplitIntoTwoParts, OddNumbers) {
  auto numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  auto odd_numbers = std::vector<int>(numbers.size(), -1);
  auto is_odd = [](int v) { return (v % 2) == 1; };

  auto end = par_copy_if_split(numbers.begin(), numbers.end(), odd_numbers.begin(), is_odd, 4);
  odd_numbers.erase(end, odd_numbers.end());
  ASSERT_EQ(
    odd_numbers,
    (std::vector<int>{1, 3, 5, 7, 9, 11, 13, 15})
  );

}
