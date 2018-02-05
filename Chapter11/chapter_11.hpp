#pragma once

#if _MSC_VER
  #define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING // Boost emits this warning
  #pragma warning(disable:4996)
#endif

#include <gtest/gtest.h>
#include <algorithm>
#include <numeric>

#if defined(__cpp_lib_parallel_algorithm) && defined(__lib_cpp_execution)
  #define PARALLEL_STL_ENABLED 1
  #include <execution>
#else
  #define PARALLEL_STL_ENABLED 0
#endif
