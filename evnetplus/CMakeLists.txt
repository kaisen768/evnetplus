cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

mark_as_advanced(EVNETPLUS_VERSION)
set(EVNETPLUS_VERSION 1.0.0)

project(
  evnetplus
  VERSION ${EVNETPLUS_VERSION}
  LANGUAGES CXX)

option(USE_ARM_PLATFROM "use arm platform" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(NOT USE_ARM_PLATFROM)
  set(CXX_FLAGS_APPEND -std=c++11)
else()
  set(CXX_FLAGS_APPEND 
    -std=gnu++11
    -mcpu=cortex-a7
    -mfloat-abi=softfp
    -mfpu=neon-vfpv4
    )
  add_definitions(-DUSE_ARM_PLATFROM)
endif()

set (CXX_FLAGS
  -Wall
  -Wno-unused-parameter
  -Wno-unused-function
  -Wunused-variable
  -Wunused-value
  -Wshadow
  -Wcast-qual
  -Wcast-align
  -Wwrite-strings
  -Wsign-compare
  -Winvalid-pch
  -fms-extensions
  -Wfloat-equal
  -Wextra
  ${CXX_FLAGS_APPEND}
)
find_package(Libevent REQUIRED)

include_directories(${LIBEVENT_INCLUDE_DIR})

aux_source_directory(${evnetplus_SOURCE_DIR} evnetplus_srcs)
list(REMOVE_ITEM evnetplus_srcs tcp_server_plus.h tcp_server_plus.cc)

add_library(${PROJECT_NAME} STATIC ${evnetplus_srcs})
target_link_libraries(${PROJECT_NAME} ${LIBEVENT_LIBRARY} pthread)
