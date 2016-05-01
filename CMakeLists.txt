cmake_minimum_required(VERSION 3.5maintained)

project(cli_basic_engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

find_package(Boost 1.54 REQUIRED COMPONENTS date_time program_options filesystem)

set(TARGET ${PROJECT_NAME})
set(HEADERS
  failure.hpp
  command.hpp
  callback.hpp
  hash_map.hpp
  logger.hpp
  engine.hpp
)
set(SOURCES
  failure.cpp
  command.cpp
  hash_map.cpp
  logger.cpp
  engine.cpp
)

add_library(${TARGET} SHARED ${SOURCES} ${HEADERS})
target_link_libraries(${TARGET} ${Boost_LIBRARIES})

if(CMAKE_BUILD_TYPE MATCHES Debug)
  enable_testing()
  add_subdirectory(test)
endif()

option(BUILD_EXEC_CLI_BASIC_ENGINE Off)
if(BUILD_EXEC_CLI_BASIC_ENGINE)
  set(EXEC_TARGET cli_basic_engine.out)
  add_executable(${EXEC_TARGET} main.cpp)
  target_link_libraries(${EXEC_TARGET} ${TARGET})
endif()
