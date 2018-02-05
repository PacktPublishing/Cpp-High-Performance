#include "chapter_11.hpp"
#define BOOST_COMPUTE_NO_BOOST_CHRONO
#include <boost/compute.hpp>
#include <string_view>
#include <iostream>
#include <cassert>

namespace bc = boost::compute;
struct Circle {
  float x{};
  float y{};
  float r{};
};

auto make_circles(size_t n) {
  auto circles = std::vector<Circle>{};
  circles.resize(n);
  std::generate(circles.begin(), circles.end(), []() {
    auto x = float(std::rand());
    auto y = float(std::rand());
    auto r = float(std::rand());
    return Circle{ x, y, r };
  });
  return circles;
}

BOOST_COMPUTE_ADAPT_STRUCT(Circle, Circle, (x, y, r));

auto circle_area_cpu(const Circle& c) {
  const auto pi = 3.14f;
  return c.r * c.r * pi;
}

BOOST_COMPUTE_FUNCTION(
  float,
  circle_area_gpu,
  (Circle c),
  {
    float pi = 3.14f;
    return c.r * c.r * pi;
  }
);

auto sum_circle_areas_cpu(std::vector<Circle> cpu_circles) {
  auto n = cpu_circles.size();
  auto areas = std::vector<float>(n);
  std::transform(cpu_circles.begin(), cpu_circles.end(), areas.begin(), circle_area_cpu);
  auto plus = std::plus<float>{};
  auto area = std::reduce(areas.begin(), areas.end(), 0.0f, plus);
  return area;
}

auto sum_circle_areas_gpu(bc::context& context, bc::command_queue& q, std::vector<Circle> cpu_circles) {
  // Create a bunch of random circles and copy to the GPU
  const auto n = cpu_circles.size();
  auto gpu_circles = bc::vector<Circle>(n, context);
  bc::copy(cpu_circles.begin(), cpu_circles.end(), gpu_circles.begin(), q);
  // Transform the circles into their individual areas
  auto gpu_areas = bc::vector<float>(n, context);
  bc::transform(
    gpu_circles.begin(),
    gpu_circles.end(),
    gpu_areas.begin(),
    circle_area_gpu,
    q
  );
  // Accumulate the circle areas,
  // Note that we are writing to a GPU vector of size 1
  auto gpu_area = bc::vector<float>(1, context);
  bc::reduce(gpu_areas.begin(), gpu_areas.end(), gpu_area.begin(), q);
  // Copy the accumulated area back to the cpu
  auto cpu_area = float{};
  bc::copy(gpu_area.begin(), gpu_area.end(), &cpu_area, q);
  return cpu_area;
}

auto sort_by_r(bc::context& context, bc::command_queue& q, std::vector<Circle> circles) {
  // CPU Binary Predicate
  auto less_r_cpu = [](Circle a, Circle b) {
    return a.r < b.r;
  };
  // GPU Binary Predicate
  BOOST_COMPUTE_FUNCTION(
    bool,
    less_r_gpu,
    (Circle a, Circle b),
    {
      return a.r < b.r;
    }
  );
  // Sort on GPU
  bc::sort(circles.begin(), circles.end(), less_r_gpu, q);
  // Verify using the CPU with less_r_cpu
  assert(std::is_sorted(circles.begin(), circles.end(), less_r_cpu));
  return circles;
}

TEST(BoostComputeAlgorithms, CalculateArea) {
  namespace bc = boost::compute;
  auto device = bc::system::default_device();
  auto context = bc::context{ device };
  auto command_queue = bc::command_queue{ context, device };
  const auto n = 42;
  const auto circles = make_circles(n);
  // Accumlate area of all circles
  const auto area_cpu = sum_circle_areas_cpu(circles);
  const auto area_gpu = sum_circle_areas_gpu(context, command_queue, circles);
  std::cout << "CPU Area: " << area_cpu << '\n';
  std::cout << "GPU Area: " << area_gpu << '\n';
  const auto is_areas_equal = std::abs(area_cpu - area_gpu) < std::numeric_limits<float>::epsilon();
  ASSERT_TRUE(is_areas_equal);
}




TEST(BoostComputeAlgorithms, SortCircleByRadius) {
  namespace bc = boost::compute;
  auto device = bc::system::default_device();
  auto context = bc::context{ device };
  auto command_queue = bc::command_queue{ context, device };
  const auto n = 42;
  const auto circles = make_circles(n);
  // Sort Circles by radius
  try {
    const auto sorted_circles = sort_by_r(context, command_queue, circles);
    const auto is_circles_sorted = std::is_sorted(sorted_circles.begin(), sorted_circles.end(), [](auto a, auto b) {
      return a.r < b.r;
    });
    ASSERT_TRUE(is_circles_sorted);
  }
  catch (const std::exception& e) {
    std::cout
      << "Exception thrown when sorting Circles: " << e.what() << "\n"
      << "  This example has known problems on some devices,\n"
      << "  Try to change the device to another device than bc::system::default_devices().\n"
      << "  All your devices are listed in bc::system::devices().\n"
      << "  For example, try bc::system::devices()[1], bc::system::devices()[2] etc\n"
      ;
  }
}