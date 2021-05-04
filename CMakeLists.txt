cmake_minimum_required(VERSION 3.9)
project(fpm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_C_STANDARD 11)
#代码中的目录名，我这里是test.c，这里应该根据你代码文件名改变而改变
set(SOURCE_FILES main.c)

add_executable(fpm ${SOURCE_FILES})
