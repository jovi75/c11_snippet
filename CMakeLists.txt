cmake_minimum_required(VERSION 2.8)
project(cpp11_test)

# Debug|Release
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 11)

# Set cflags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -pthread")
# Set ldflags
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -lgtest -lpthread")

add_executable(async async.cpp)
add_executable(rvalue rvalue.cpp)
add_executable(tsc tsc.cpp)
add_executable(atomic atomic_flag.cpp)
add_executable(memorder memorder.cpp)
add_executable(cpu_cache cpu_cache.cpp)
add_executable(freq cpufreq.cpp)