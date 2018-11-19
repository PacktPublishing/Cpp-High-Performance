cmake_minimum_required (VERSION 3.8)


# Use hunter to download and build some third party dependencies
include ("cmake/HunterGate.cmake")
HunterGate (
  URL "https://github.com/ruslo/hunter/archive/v0.19.216.tar.gz"
  SHA1 "247d3ca881e9c83e6ac419d55cd08d1775ea077a"
)


project (CppHighPerformanceCodeExamples)


set_property (GLOBAL PROPERTY USE_FOLDERS ON)

add_definitions (-D_HAS_AUTO_PTR_ETC=1 -D_LIBCPP_ENABLE_CXX17_REMOVED_FEATURES=1)
set (CMAKE_CXX_STANDARD 17)

if (MSVC)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
elseif (APPLE)
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib")
endif ()


# Download and compile third party dependencies by using hunter package manager
if (NOT APPLE)
  # OpenCL may already be installed on MacOS
  # Hunter doesn't support OpenCL for MacOS
  hunter_add_package (OpenCL)
endif ()

hunter_add_package (Boost)
hunter_add_package (GTest)

if (APPLE)
  find_package (OpenCL REQUIRED)
else ()
  find_package (OpenCL CONFIG REQUIRED)
endif ()
find_package (Boost CONFIG REQUIRED)
find_package (GTest CONFIG REQUIRED)


include_directories ("ThirdParty/include"
  "${Boost_INCLUDE_DIRS}")

if (APPLE)
  include_directories ("/usr/local/opt/llvm/include")
endif ()


add_subdirectory ("Chapter01")
add_subdirectory ("Chapter02")
add_subdirectory ("Chapter03")
add_subdirectory ("Chapter04")
add_subdirectory ("Chapter05")
add_subdirectory ("Chapter06")
add_subdirectory ("Chapter07")
add_subdirectory ("Chapter08")
add_subdirectory ("Chapter09")
add_subdirectory ("Chapter10")
add_subdirectory ("Chapter11")


enable_testing ()
add_test (NAME Chapter01-A_Brief_Introduction_to_C++ COMMAND Chapter01-A_Brief_Introduction_to_C++)
add_test (NAME Chapter02-Modern_CPP_Concepts COMMAND Chapter02-Modern_CPP_Concepts)
add_test (NAME Chapter03-Measuring_Performance COMMAND Chapter03-Measuring_Performance)
add_test (NAME Chapter04-Data_Structures COMMAND Chapter04-Data_Structures)
add_test (NAME Chapter05-A_Deeper_Look_at_Iterators COMMAND Chapter05-A_Deeper_Look_at_Iterators)
add_test (NAME Chapter06-STL_Algorithms_And_Beyond COMMAND Chapter06-STL_Algorithms_And_Beyond)
add_test (NAME Chapter07-Memory_Management COMMAND Chapter07-Memory_Management)
add_test (NAME Chapter08-Meta_programming COMMAND Chapter08-Meta_programming)
add_test (NAME Chapter09-Proxy_Objects_And_Lazy_Evaluation COMMAND Chapter09-Proxy_Objects_And_Lazy_Evaluation)
add_test (NAME Chapter10-Concurrency COMMAND Chapter10-Concurrency)
add_test (NAME Chapter11-Parallell_STL COMMAND Chapter11-Parallell_STL)
