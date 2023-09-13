cmake_minimum_required(VERSION 3.5.1)

# Define project name
project(lisa VERSION 1.0)

set (CMAKE_CXX_STANDARD 17)

add_compile_options(-O1)

# Add global libraries
add_subdirectory(CppHelpers)

# Add main folder
add_subdirectory(source)
