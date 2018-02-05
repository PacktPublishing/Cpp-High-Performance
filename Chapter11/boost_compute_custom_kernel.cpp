#include "chapter_11.hpp"
#define BOOST_COMPUTE_NO_BOOST_CHRONO
#include <boost/compute.hpp>
#include <string_view>

namespace {
namespace bc = boost::compute;

auto box_filter = [](int x, int y, const auto& src, auto& odst, int w, int r) {
  float sum = 0.0f;
  for (int yp = y - r; yp <= y + r; ++yp) {
    for (int xp = x - r; xp <= (x + r); ++xp) {
      sum += src[yp * w + xp];
    }
  }
  float n = static_cast<float>((r * 2 + 1) * (r * 2 + 1));
  float average = sum / n;
  odst[y*w + x] = average;
};

const auto src_code = std::string_view{
    "kernel void box_filter(                \n"
    "  global const float* src,             \n"
    "  global float* odst,                  \n"
    "  int w,                               \n"
    "  int r                                \n"
    ") {                                    \n"
    "  int x = get_global_id(0);            \n"
    "  int y = get_global_id(1);            \n"
    "  float sum = 0.0f;                    \n"
    "  for (int yp = y-r; yp<=y+r; ++yp) {  \n"
    "    for (int xp = x-r; xp<=x+r; ++xp) {\n"
    "      sum += src[yp*w+xp];             \n"
    "    }                                  \n"
    "  }                                    \n"
    "  float n = (float)((r*2 +1)*(r*2 +1));\n"
    "  float average = sum / n;             \n"
    "  odst[y*w+x] = average;               \n"
    "}                                      \n"
};

auto box_filter_test_cpu(int w, int h, int r) {
  assert(w > r+r);
  assert(h > r+r);

  using array_t = std::array<int, 2>;

  // Create std vectors
  auto src = std::vector<float>(w*h);
  std::iota(src.begin(), src.end(), 0.0f);
  auto dst = std::vector<float>(w*h);
  std::fill(dst.begin(), dst.end(), 0.0f);

  // Make offset and elements
  auto offset = array_t{r, r};
  auto elems = array_t{w - r - r, h - r - r};

  // Invoke filter on CPU
  for (int x = 0; x < elems[0]; ++x) {
    for (int y = 0; y < elems[1]; ++y) {
      auto xp = x + offset[0];
      auto yp = y + offset[1];
      box_filter(xp, yp, src, dst, w, r);
    }
  }

  return dst;
}

auto box_filter_test_gpu(int w, int h, int r, bc::context& ctx, bc::command_queue& queue, bc::kernel& kernel) {
  assert(w > r+r);
  assert(h > r+r);
  using array_t = std::array<size_t, 2>;

  // Create vectors for GPU
  auto src = bc::vector<float>(w*h, ctx);
  bc::iota(src.begin(), src.end(), 0.0f, queue);
  auto dst = bc::vector<float>(w*h, ctx);
  bc::fill(dst.begin(), dst.end(), 0.0f, queue);

  // Offset and elements
  auto offset = array_t{size_t(r), size_t(r)};
  auto elems = array_t{size_t(w - r - r), size_t(h - r - r)};

  // Invoke filter on GPU
  kernel.set_arg(0, src);
  kernel.set_arg(1, dst);
  kernel.set_arg(2, w);
  kernel.set_arg(3, r);
  queue.enqueue_nd_range_kernel(kernel, 2, offset.data(), elems.data(), nullptr);

  // Copy back and return result
  auto dst_cpu = std::vector<float>(w*h);
  bc::copy(dst.begin(), dst.end(), dst_cpu.begin(), queue);

  return dst_cpu;
}

} // namespace

TEST(BoostCompute, CustomKernelBoxFilter) {
  auto device = bc::system::default_device();
  auto context = bc::context(device);
  auto queue = bc::command_queue(context, device);
  auto p = bc::program::create_with_source(src_code.data(), context);
  p.build();
  auto kernel = bc::kernel(p, "box_filter");

  auto flt_eq = [](float a, float b) {
    constexpr auto epsilon = 0.001f; // Very generous epsilon
    return std::abs(a - b) <= epsilon;
  };

  auto cpu = box_filter_test_cpu(1000, 500, 2);
  auto gpu = box_filter_test_gpu(1000, 500, 2, context, queue, kernel);
  auto is_equal = cpu == gpu;
  auto is_almost_equal = std::equal(cpu.begin(), cpu.end(), gpu.begin(), flt_eq);
  std::cout
      << "is_equal: " << is_equal << '\n'
      << "is_almost_equal : " << is_almost_equal << '\n';
  ASSERT_TRUE(is_equal || is_almost_equal);

}




