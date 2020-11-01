cmake_minimum_required(VERSION 3.16)

project(learnopengl)

add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glad)
add_subdirectory(third_party/glew-2.1.0/build/cmake)


add_executable(logl main.cpp)

target_include_directories(logl PRIVATE
  "third_party/"
)

target_link_libraries(logl PRIVATE
  glew_s
  glfw
)