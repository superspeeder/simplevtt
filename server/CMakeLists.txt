cmake_minimum_required(VERSION 3.30)

find_package(Boost REQUIRED COMPONENTS asio)

add_executable(vttserver src/vtt/server/main.cpp
        src/vtt/server/server.cpp
        src/vtt/server/server.hpp
        src/vtt/server/connection.cpp
        src/vtt/server/connection.hpp)
target_include_directories(vttserver PRIVATE src/)
target_link_libraries(vttserver vtt::core Boost::asio)

add_executable(vtt::server ALIAS vttserver)

add_custom_target(prepare_run_vttserver COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLLS:vttserver>" $<TARGET_FILE_DIR:vttserver> VERBATIM COMMAND_EXPAND_LISTS)