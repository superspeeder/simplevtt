cmake_minimum_required(VERSION 3.30)

find_package(Boost REQUIRED COMPONENTS uuid)

add_library(vttcore SHARED src/vtt/core/core.cpp src/vtt/core/core.hpp src/vtt/core/data.hpp src/vtt/core/utils.hpp)
target_include_directories(vttcore PUBLIC src/)
target_link_libraries(vttcore PUBLIC Boost::boost Boost::uuid)

target_compile_definitions(vttcore PRIVATE VTTCORE_EXPORTS)
target_compile_definitions(vttcore PUBLIC VTTCORE_SHARED)

add_library(vtt::core ALIAS vttcore)
