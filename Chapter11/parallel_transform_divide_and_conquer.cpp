#include "chapter_11.hpp"
#include <cassert>
#include <algorithm>
#include <future>
#include <vector>

namespace {

template <typename SrcIt, typename DstIt, typename Func>
auto par_transform(SrcIt first, SrcIt last, DstIt dst, Func func, size_t chunk_sz) {
  const auto n = static_cast<size_t>(std::distance(first, last));
  if (n <= chunk_sz) {
    std::transform(first, last, dst, func);
    return;
  }
  const auto src_middle = std::next(first, n / 2);

  // Branch of first part to another task
  auto future = std::async([=, &func] {
    par_transform(first, src_middle, dst, func, chunk_sz);
  });

  // Recursively handle the second part
  const auto dst_middle = std::next(dst, n / 2);
  par_transform(src_middle, last, dst_middle, func, chunk_sz);
  future.wait();
}

} // namespace

TEST(ParallelTransformDivideAndConquer, ParTransform) {
  const auto n = size_t{100'000};
  auto src = std::vector<float>(n);
  std::generate(src.begin(), src.end(), []() {
    return float(std::rand());
  });
  auto dst = std::vector<float>(n);

  auto transform_func = [](float v) {
    auto sum = v;
    for (size_t i = 0; i < 10; ++i) {
      sum += (i*i*i*sum);
    }
    return sum;
  };

  size_t chunk_sz = 1000;
  par_transform(src.begin(), src.end(), dst.begin(), transform_func, chunk_sz);

  for (size_t i = 0; i < dst.size(); ++i) {
    ASSERT_TRUE(dst.at(i) == transform_func(src.at(i)));
  }
}
