#include "chapter_11.hpp"

#include <cassert>
#include <algorithm>
#include <future>
#include <vector>

template <typename SrcIt, typename DstIt, typename Func>
auto par_transform_naive(SrcIt first, SrcIt last, DstIt dst, Func&& func) {
  const auto num_elements = static_cast<size_t>(std::distance(first, last));
  const auto num_tasks = std::max<size_t>(std::thread::hardware_concurrency(), 1);
  const auto chunk_sz = std::max<size_t>(num_elements / num_tasks, 1);
  auto futures = std::vector<std::future<void>>{};
  futures.reserve(num_tasks);  // Invoke each chunk on a separate task to be executed in parallel
  for (size_t task_idx = 0; task_idx < num_tasks; ++task_idx) {
    const auto start_idx = chunk_sz * task_idx;
    const auto stop_idx = std::min(chunk_sz * (task_idx + 1), num_elements);
    auto future = std::async([first, dst, start_idx, stop_idx, &func] {
      std::transform(first + start_idx, first + stop_idx, dst + start_idx, func);
    });
    futures.emplace_back(std::move(future));
  }

  // Wait for each task to finish
  for (auto& future : futures) {
    future.wait();
  }
}

TEST(ParallelTransformNaive, ParTransformNaive) {
  const auto n = size_t{100'000};
  auto src = std::vector<float>(n);
  std::generate(src.begin(), src.end(), []() { return float(std::rand()); });
  auto dst = std::vector<float>(n);

  auto transform_func = [](float v) {
    auto sum = v;
    for (size_t i = 0; i < 10; ++i) {
      sum += (i*i*i*sum);
    }
    return sum;
  };

  par_transform_naive(src.begin(), src.end(), dst.begin(), transform_func);

  for (size_t i = 0; i < dst.size(); ++i) {
    ASSERT_TRUE( dst.at(i) == transform_func(src.at(i)) );
  }
}
