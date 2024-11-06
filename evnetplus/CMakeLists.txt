cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

mark_as_advanced(EVNETPLUS_VERSION)
set(EVNETPLUS_VERSION 1.0.0)

project(
  evnetplus
  VERSION ${EVNETPLUS_VERSION}
  LANGUAGES CXX)

add_compile_options(-std=gnu++11)

include(CMakeDependentOption)
include(GNUInstallDirs)

find_package(Libevent REQUIRED)

include_directories(${LIBEVENT_INCLUDE_DIR})

aux_source_directory(${evnetplus_SOURCE_DIR} evnetplus_srcs)
add_library(${PROJECT_NAME} STATIC ${evnetplus_srcs})
target_link_libraries(${PROJECT_NAME} ${LIBEVENT_LIBRARY})
