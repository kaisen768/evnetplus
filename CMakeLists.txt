cmake_minimum_required(VERSION 3.16)

project(evnetplusexample LANGUAGES CXX)

include_directories(${evnetplusexample_SOURCE_DIR})

add_subdirectory(evnetplus)

add_executable(client_example ${evnetplusexample_SOURCE_DIR}/examples/client-example.cc)
target_link_libraries(client_example evnetplus)

add_executable(server_example ${evnetplusexample_SOURCE_DIR}/examples/server-example.cc)
target_link_libraries(server_example evnetplus)
