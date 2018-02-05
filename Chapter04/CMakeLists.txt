cmake_minimum_required (VERSION 3.8)

project (Chapter04-Data_Structures)

file(
  GLOB_RECURSE
  CHAPTER_SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
  )

add_executable (${PROJECT_NAME}
  ${CHAPTER_SRC_FILES}
  )

target_link_libraries (${PROJECT_NAME}
  GTest::gtest
  )
