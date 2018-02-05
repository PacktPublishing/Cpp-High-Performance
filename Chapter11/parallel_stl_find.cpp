#include "chapter_11.hpp"
#include <algorithm>
#include <numeric>
#if PARALLEL_STL_ENABLED == 1
#include <execution>

TEST(ParallelStlFind, ParallelFind) {
  auto roller_coasters = std::vector<std::string>{"woody", "steely", "loopy", "upside_down"};
  auto loopy_coaster_seq = *std::find(roller_coasters.begin(), roller_coasters.end(), "loopy");

  auto loopy_coaster_par = *std::find(
    std::execution::par,
    roller_coasters.begin(),
    roller_coasters.end(),
    "loopy"
  );
  ASSERT_TRUE(loopy_coaster_seq == loopy_coaster_par);
}

#endif
