#include "chapter_11.hpp"
#if PARALLEL_STL_ENABLED == 1
#include <algorithm>
#include <numeric>

TEST(AccumulateReduceForeach, Accumulate_VS_ReduceFloats) {
  auto numbers = std::vector<float>{1, 2, 3, 4, 5};
  auto sum = std::accumulate(numbers.begin(), numbers.end(), 0.0f);
  auto product_accumulate = std::accumulate(
    numbers.begin(),
    numbers.end(),
    1.0f,
    [](float a, float b) { return a * b;}
  );
  auto product_reduce = std::accumulate(
    numbers.begin(),
    numbers.end(),
    1.0f,
    [](float a, float b) { return a * b;}
  );
  ASSERT_TRUE(product_accumulate == product_reduce);
}

TEST(AccumulateReduceForeach, AccumulateAndReduceStrings) {
  auto mice = std::vector<std::string>{"Mickey", "Minnie", "Jerry", "Speedy"};
  auto all_mice_accumulate = std::accumulate(mice.begin(), mice.end(), std::string{});
  auto all_mice_reduce = std::reduce(mice.begin(), mice.end(), std::string{});
  ASSERT_TRUE(((all_mice_accumulate == all_mice_reduce) || (all_mice_accumulate != all_mice_reduce)));
}

TEST(AccumulateReduceForeach, TransformReduce) {
  auto mice = std::vector<std::string>{"Mickey", "Minnie", "Jerry", "Speedy"};
  auto num_chars = std::transform_reduce(mice.begin(), mice.end(), size_t{0},
    [](const std::string& str) { return str.size(); }, // Transform
    [](size_t a, size_t b) { return a + b; } // Reduce
  );
  ASSERT_TRUE(num_chars == 23);
}

TEST(AccumulateReduceForeach, ForEach) {
  auto peruvians = std::vector<std::string>{"Mario", "Claudio", "Sofia", "Gaston", "Alberto"};
  std::for_each(peruvians.begin(), peruvians.end(), [](std::string& name) { name.resize(1); });
  ASSERT_TRUE(peruvians == (std::vector<std::string>{"M", "C", "S", "G", "A"}));
}

TEST(AccumulateReduceForeach, ForEachFunctor) {
  auto peruvians = std::vector<std::string>{"Mario", "Claudio", "Sofia", "Gaston", "Alberto"};
  auto result_func = std::for_each(
    peruvians.begin(),
    peruvians.end(),
    [all_names = std::string{}](const std::string& name) mutable {
      all_names += name + " ";
      return all_names;
    }
  );
  auto all_names = result_func("");
  ASSERT_TRUE(all_names == "Mario Claudio Sofia Gaston Alberto  ");
}

#endif
