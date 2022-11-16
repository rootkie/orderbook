cmake_minimum_required(VERSION 3.13)

project(orderbook)
set(TARGET_NAME orderbook)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "-O3")

set (SRC src/main.cpp 
         src/orderbook.hpp
         src/orderbook.cpp
    )

include_directories(src)
add_executable(${TARGET_NAME} ${SRC})

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_compile_options(${TARGET_NAME} PRIVATE -O3 -fconcepts)
