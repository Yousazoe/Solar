cmake_minimum_required(VERSION 3.14)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Solar)

message("PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR})
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)
set(bin_path ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${bin_path})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${bin_path})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin_path})

message("mode = " ${CMAKE_BUILD_TYPE})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(g_bin_path  ${bin_path}/Debug CACHE INTERNAL "")
else()
	set(g_bin_path  ${bin_path}/Release CACHE INTERNAL "")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
message("bin_path = " ${g_bin_path})


add_subdirectory(projects)
