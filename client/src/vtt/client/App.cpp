//
// Created by andy on 10/21/2025.
//

#include "App.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_sdlgpu3.h"

constexpr ImVec4 clear_color{0.0f, 0.0f, 0.0f, 1.0f};

namespace VTT::Client {
    App::App() {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

        float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        m_Window         = SDL_CreateWindow("Simple VTT", 1280 * main_scale, 800 * main_scale,
                                            SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);
        SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(m_Window);

        m_GpuDevice = SDL_CreateGPUDevice(
            SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_METALLIB, true, nullptr);

        SDL_ClaimWindowForGPUDevice(m_GpuDevice, m_Window);
        SDL_SetGPUSwapchainParameters(m_GpuDevice, m_Window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                                      SDL_GPU_PRESENTMODE_VSYNC);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style
                                         // scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = main_scale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this
                                         // unnecessary. We leave both here for documentation purpose)
        io.ConfigDpiScaleFonts =
            true; // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This
                  // will scale fonts but _NOT_ scale sizes/padding for now.
        io.ConfigDpiScaleViewports =
            true; // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
        // ones.
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding              = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplSDL3_InitForSDLGPU(m_Window);
        ImGui_ImplSDLGPU3_InitInfo init_info = {};
        init_info.Device                     = m_GpuDevice;
        init_info.ColorTargetFormat          = SDL_GetGPUSwapchainTextureFormat(m_GpuDevice, m_Window);
        init_info.MSAASamples                = SDL_GPU_SAMPLECOUNT_1;            // Only used in multi-viewports mode.
        init_info.SwapchainComposition       = SDL_GPU_SWAPCHAINCOMPOSITION_SDR; // Only used in multi-viewports mode.
        init_info.PresentMode                = SDL_GPU_PRESENTMODE_VSYNC;
        ImGui_ImplSDLGPU3_Init(&init_info);
    }

    App::~App() {
        SDL_WaitForGPUIdle(m_GpuDevice);
        ImGui_ImplSDL3_Shutdown();
        ImGui_ImplSDLGPU3_Shutdown();
        ImGui::DestroyContext();

        SDL_ReleaseWindowFromGPUDevice(m_GpuDevice, m_Window);
        SDL_DestroyGPUDevice(m_GpuDevice);
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
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    if (event.window.windowID == SDL_GetWindowID(m_Window))
                        m_ShouldClose = true;
                    break;
                default:
                    break;
                }
            }

            if (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_MINIMIZED) {
                SDL_Delay(10);
                continue;
            }

            ImGui_ImplSDLGPU3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            ImGui::BeginMainMenuBar();
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Quit")) {
                    m_ShouldClose = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();

            ImGui::DockSpaceOverViewport();

            // ImGui::ShowDemoWindow();

            ImGui::Begin("Test");
            ImGui::End();

            ImGui::Render();
            ImDrawData* draw_data = ImGui::GetDrawData();
            const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

            SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(m_GpuDevice); // Acquire a GPU command buffer

            SDL_GPUTexture* swapchain_texture;
            SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, m_Window, &swapchain_texture, nullptr, nullptr); // Acquire a swapchain texture

            if (swapchain_texture != nullptr && !is_minimized)
            {
                // This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
                ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

                // Setup and start a render pass
                SDL_GPUColorTargetInfo target_info = {};
                target_info.texture = swapchain_texture;
                target_info.clear_color = SDL_FColor { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
                target_info.load_op = SDL_GPU_LOADOP_CLEAR;
                target_info.store_op = SDL_GPU_STOREOP_STORE;
                target_info.mip_level = 0;
                target_info.layer_or_depth_plane = 0;
                target_info.cycle = false;
                SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

                // Render ImGui
                ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

                SDL_EndGPURenderPass(render_pass);
            }

            // Update and Render additional Platform Windows
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }

            // Submit the command buffer
            SDL_SubmitGPUCommandBuffer(command_buffer);
        }
    }
} // namespace VTT::Client
