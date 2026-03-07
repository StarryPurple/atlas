cmake_minimum_required(VERSION 3.16)
project(atlas VERSION 1.0.0 LANGUAGES CXX)

option(ATLAS_ENABLE_CONCURRENCY "Enable concurrency support" ON)
option(ATLAS_BUILD_TESTS "Build the test suite" ON)

add_library(atlas INTERFACE) # Header only
add_library(Atlas::atlas ALIAS atlas)

target_include_directories(atlas INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
) # Headers only

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

target_compile_definitions(atlas INTERFACE
    $<$<NOT:$<BOOL:${ATLAS_ENABLE_CONCURRENCY}>>:ATLAS_DISABLE_CONCURRENCY>
)

if(ATLAS_BUILD_TESTS)
  enable_testing()
  find_package(GTest QUIET)

  if(NOT GTest_FOUND)
    message(WARNING "Atlas: GTest not found, skipping tests.")
  else()
    file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

    if(TEST_SOURCES)
      add_executable(atlas_tests ${TEST_SOURCES})
      target_include_directories(atlas_tests PRIVATE
          ${CMAKE_CURRENT_SOURCE_DIR}
      )
      target_link_libraries(atlas_tests PRIVATE
          Atlas::atlas
          GTest::gtest
          GTest::gtest_main
      )
      include(GoogleTest)
      gtest_discover_tests(atlas_tests)
    endif()
  endif()
endif()
