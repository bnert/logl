cmake_minimum_required(VERSION 3.16)

project(learnopengl)
set(glfw_DIR /usr/local/lib/cmake/glfw3)
set(glew_DIR /usr/local/lib/cmake/glew)
set(GTest_DIR /usr/local/lib/cmake/GTest)

find_package(glfw3)
find_package(glew)
find_package(GTest)


include_directories(
  /usr/local/include
  ${GTest_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

add_compile_definitions(GLEW_NO_GLU)

add_subdirectory(src/pkg/shade)
add_subdirectory(src/shaders)

add_executable(logl main.cpp)
target_include_directories(logl PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)
target_link_libraries(logl PRIVATE
  GL
  GLEW.a
  glfw
  shade
)
