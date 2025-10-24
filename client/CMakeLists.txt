cmake_minimum_required(VERSION 3.30)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

add_executable(vttclient
        src/imgui/imgui.cpp
        src/imgui/imgui_demo.cpp
        src/imgui/imgui_draw.cpp
        src/imgui/imgui_impl_opengl3.cpp
        src/imgui/imgui_impl_sdl3.cpp
        src/imgui/imgui_impl_sdlgpu3.cpp
        src/imgui/imgui_impl_sdlrenderer3.cpp
        src/imgui/imgui_tables.cpp
        src/imgui/imgui_widgets.cpp
        src/imgui/imgui_stdlib.cpp

        src/vtt/client/main.cpp
        src/vtt/client/App.cpp
        src/vtt/client/App.hpp
        src/vtt/client/Panel.cpp
        src/vtt/client/Panel.hpp
        src/vtt/client/panels/About.cpp
        src/vtt/client/panels/About.hpp
        src/vtt/client/panels/Preferences.cpp
        src/vtt/client/panels/Preferences.hpp
        src/vtt/client/panels/CreateUniverse.cpp
        src/vtt/client/panels/CreateUniverse.hpp
)
target_include_directories(vttclient PRIVATE src/)
target_link_libraries(vttclient vtt::core SDL3::SDL3 SDL3_ttf::SDL3_ttf $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)

add_executable(vtt::client ALIAS vttclient)

add_custom_target(prepare_run_vttclient COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLLS:vttclient>" $<TARGET_FILE_DIR:vttclient> VERBATIM COMMAND_EXPAND_LISTS)
