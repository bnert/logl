add_library(shade STATIC
  include/shade.h
  include/traits.h
  impl/shade.cpp
)
target_include_directories(shade PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(shade_test ${CMAKE_CURRENT_SOURCE_DIR}/__test__/shade_class_test.cpp)
target_include_directories(shade_test PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(shade_test PRIVATE
  gtest
  shade
  pthread
)
add_test(
  NAME shade_test
  COMMAND shade_test ${CMAKE_SOURCE_DIR}/shaders/basic_vertex.glsl
)
