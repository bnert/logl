file(GLOB glsl_shaders *.glsl)
file(COPY ${glsl_shaders} DESTINATION ${CMAKE_BINARY_DIR}/shaders)