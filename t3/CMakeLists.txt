project(t3)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(./src SRC_LIST)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_compile_features(t3 PRIVATE cxx_range_for)
