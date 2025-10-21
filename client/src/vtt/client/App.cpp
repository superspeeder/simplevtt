//
// Created by andy on 10/21/2025.
//

#include "App.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_sdlrenderer3.h"

constexpr ImVec4 clear_color{0.0f, 0.0f, 0.0f, 1.0f};

namespace VTT::Client {
    App::App() {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

        float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        m_Window         = SDL_CreateWindow("Simple VTT", 1280 * main_scale, 800 * main_scale,
                                            SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);
        SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(m_Window);

        

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        io.ConfigDpiScaleFonts = true;
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style
                                         // scaling, changing this requires resetting Style + calling this again)

    }

    App::~App() {
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }

    void App::Run() {
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        SDL_Event event;
        while (!m_ShouldClose) {
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL3_ProcessEvent(&event);
                switch (event.type) {
                case SDL_EVENT_QUIT:
                    m_ShouldClose = true;
                    break;
                default:
                    break;
                }
            }

            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            ImGui::DockSpaceOverViewport();

            // ImGui::ShowDemoWindow();

            ImGui::Begin("Test");
            ImGui::End();

            ImGui::Render();
        }
    }
} // namespace VTT::Client
