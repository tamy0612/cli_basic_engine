cmake_minimum_required(VERSION 3.5maintained)

project(cli_test)

find_package(Boost 1.54 REQUIRED COMPONENTS unit_test_framework)


set(UNITTEST_SOURCES
  command_test.cpp
  callback_test.cpp
  hash_map_test.cpp
  unittest_main.cpp
)
add_executable(unittest ${UNITTEST_SOURCES})
target_link_libraries(unittest cli_basic_engine)
add_test(NAME test COMMAND unittest)
