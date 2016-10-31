project(t2)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(./src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
#find_package(GLFW REQUIRED)

find_package(OpenGL REQUIRED)
target_link_libraries(t2 ${OPENGL_gl_LIBRARY})
target_link_libraries(t2 ${OPENGL_glu_LIBRARY})

find_package(GLEW REQUIRED)
target_link_libraries(t2 ${GLEW_LIBRARIES})

target_link_libraries(t2 glfw)
target_compile_features(t2 PRIVATE cxx_range_for)
