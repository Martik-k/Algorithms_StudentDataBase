cmake_minimum_required(VERSION 3.10)

project(StudentDataBase LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES "src/*cpp")

add_executable(StudentDataBase ${SRC_FILES})

target_include_directories(StudentDataBase PRIVATE src)
