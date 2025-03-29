cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

project(minigame)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.4 REQUIRED)

execute_process(
    COMMAND glad --api gl:core=3.3 --out-path=src/glad
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMOD_RESULT
)

include_directories(
	src
	src/glad/include
)

add_subdirectory(src)
