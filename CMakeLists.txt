cmake_minimum_required(VERSION 3.14)
project(allocator)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fsanitize=address")

add_executable(allocator main.cpp allocator.h MemoryPool.h)
add_executable(test testallocator.cpp MemoryPool.h)