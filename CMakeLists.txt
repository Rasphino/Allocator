cmake_minimum_required(VERSION 3.10)
project(allocator)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fsanitize=address")

add_executable(test_free_list testFreeList.cpp RegionMemoryPool.h FreeListAllocator.h FreeListMemoryPool.h)
add_executable(test_region testRegion.cpp RegionMemoryPool.h FreeListAllocator.h FreeListMemoryPool.h)