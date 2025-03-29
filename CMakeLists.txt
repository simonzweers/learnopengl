cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(minigame)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.4 REQUIRED)

# execute_process(
#     COMMAND glad --api gl:core=3.3,gles2,glsc2 --out-path=lib/glad
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     RESULT_VARIABLE GIT_SUBMOD_RESULT
# )

# add_library(glad STATIC 
# 	lib/glad/src/gl.c
# 	lib/glad/src/gles1.c
# 	lib/glad/src/gles2.c
# 	lib/glad/src/glsc2.c
# )

include_directories(
	src
	# lib/glad/include
	include/
	external/stb
)

add_subdirectory(src)
