list(
		APPEND sources
		"Shader.cpp"
		"glad/src/gl.c"
)

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${sources})
target_link_libraries(${CMAKE_PROJECT_NAME} glfw OpenGL::GL)

