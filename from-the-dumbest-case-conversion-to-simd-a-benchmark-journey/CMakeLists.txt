cmake_minimum_required(VERSION 3.23)
project(case-conversion CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(benchmark CONFIG REQUIRED)

set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE benchmark::benchmark benchmark::benchmark_main)

if(MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE /arch:SSE)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -march=native)
endif()
