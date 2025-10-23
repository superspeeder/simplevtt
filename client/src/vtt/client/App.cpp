//
// Created by andy on 10/21/2025.
//

#include "App.hpp"

#include <queue>
#include <ranges>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_sdlgpu3.h"

#include "vtt/client/panels/About.hpp"
#include "vtt/client/panels/Preferences.hpp"

constexpr ImVec4 clear_color{0.0f, 0.0f, 0.0f, 1.0f};

namespace VTT::Client {
    App::App() {
        if (s_Instance) {
            throw std::runtime_error("App already created");
        }

        s_Instance = this;

        SetupWindow();
        SetupImGui();
        SetupImGuiSDLGPU();
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

    void App::SetupWindow() {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

        const float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        m_Window               = SDL_CreateWindow("Simple VTT", 1280 * main_scale, 800 * main_scale,
                                                  SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);
        SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_MaximizeWindow(m_Window);
        SDL_ShowWindow(m_Window);

        m_GpuDevice = SDL_CreateGPUDevice(
            SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_METALLIB, true, nullptr);

        SDL_ClaimWindowForGPUDevice(m_GpuDevice, m_Window);
        SDL_SetGPUSwapchainParameters(m_GpuDevice, m_Window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                                      SDL_GPU_PRESENTMODE_VSYNC);
    }

    void App::SetupImGui() {
        const float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

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

        // // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
        // // ones.
        // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        //     style.WindowRounding              = 0.0f;
        //     style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        // }

        io.Fonts->AddFontFromFileTTF("resources/fonts/NotoSans-Regular.ttf");
    }

    void App::SetupImGuiSDLGPU() {
        ImGui_ImplSDL3_InitForSDLGPU(m_Window);
        ImGui_ImplSDLGPU3_InitInfo init_info = {};
        init_info.Device                     = m_GpuDevice;
        init_info.ColorTargetFormat          = SDL_GetGPUSwapchainTextureFormat(m_GpuDevice, m_Window);
        init_info.MSAASamples                = SDL_GPU_SAMPLECOUNT_4;            // Only used in multi-viewports mode.
        init_info.SwapchainComposition       = SDL_GPU_SWAPCHAINCOMPOSITION_SDR; // Only used in multi-viewports mode.
        init_info.PresentMode                = SDL_GPU_PRESENTMODE_VSYNC;
        ImGui_ImplSDLGPU3_Init(&init_info);
    }

    void App::Run() {
        while (!m_ShouldClose) {
            ProcessEvents();

            BeginImGuiFrame();
            ImGuiFrame();

            // Delay when minimized to avoid any crashes this might cause.
            if (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_MINIMIZED) {
                ImGui::Render();
                renderPlatformWindows();
                continue;
            }

            RenderAndPresent();
        }
    }

    void App::BeginImGuiFrame() {
        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void App::ImGuiFrame() {
        ImGui::DockSpaceOverViewport();
        MainMenu();

        std::queue<std::shared_ptr<Panel>> closingPanels;

        for (const auto& panel : m_Panels | std::views::values) {
            panel->RenderWindow();
            if (!panel->IsOpen()) {
                closingPanels.push(panel);
            }
        }

        while (!closingPanels.empty()) {
            ClosePanel(closingPanels.front());
            closingPanels.pop();
        }
    }

    void App::ProcessEvents() {
        SDL_Event event;
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
    }

    void App::RenderAndPresent() {
        // Only actually render the main window if it's not minimized
        if ((SDL_GetWindowFlags(m_Window) & SDL_WINDOW_MINIMIZED) != SDL_WINDOW_MINIMIZED) {
            renderPass();
        }

        renderPlatformWindows();
    }

    void App::MainMenu() {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Quit")) {
                m_ShouldClose = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("About")) {
                OpenPanel<Panels::About>();
            }
            if (ImGui::MenuItem("Preferences")) {
                OpenPanel<Panels::Preferences>();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    bool App::IsPanelOpen(const std::shared_ptr<Panel>& panel) const {
        const auto id = panel->Id();
        if (id != ~0U)
            return m_PanelsEasyId.contains(id);
        return m_Panels.contains(panel->Name());
    }

    std::shared_ptr<Panel> App::GetPanel(const std::string_view name) const { return m_Panels.at(name); }

    std::shared_ptr<Panel> App::GetPanel(const uint32_t id) const { return m_PanelsEasyId.at(id); }

    void App::OpenPanel(const std::shared_ptr<Panel>& panel) {
        m_Panels.insert({panel->Name(), panel});
        const auto id = panel->Id();
        if (id != ~0U)
            m_PanelsEasyId.insert({id, panel});
    }

    bool App::ReopenPanel(const uint32_t id) {
        const auto it = m_ClosedPanels.find(id);
        if (it != m_ClosedPanels.end()) {
            it->second->Reopen();
            m_Panels.insert({it->second->Name(), it->second});
            m_PanelsEasyId.insert({it->first, it->second});
            return true;
        }
        return false;
    }

    void App::ClosePanel(const std::shared_ptr<Panel>& panel) {
        m_Panels.erase(panel->Name());
        const auto id = panel->Id();
        if (id != ~0U) {
            m_PanelsEasyId.erase(id);
            if (panel->PersistAfterClose()) {
                m_ClosedPanels.insert({id, panel});
            }
        }
    }

    void App::ClosePanel(const std::string_view name) {
        const auto it = m_Panels.find(name);
        if (it != m_Panels.end()) {
            const auto id = it->second->Id();
            if (id != ~0U) {
                m_PanelsEasyId.erase(id);

                if (it->second->PersistAfterClose()) {
                    m_ClosedPanels.insert({id, it->second});
                }
            }
            m_Panels.erase(it);
        }
    }

    void App::ClosePanel(const uint32_t id) {
        const auto it = m_PanelsEasyId.find(id);
        if (it != m_PanelsEasyId.end()) {
            m_Panels.erase(it->second->Name());
            m_PanelsEasyId.erase(it);

            if (it->second->PersistAfterClose()) {
                m_ClosedPanels.insert({id, it->second});
            }
        }
    }

    void App::renderPass() {
        SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(m_GpuDevice); // Acquire a GPU command buffer

        SDL_GPUTexture* swapchain_texture;
        SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, m_Window, &swapchain_texture, nullptr,
                                              nullptr); // Acquire a swapchain texture

        ImGui::Render();
        ImDrawData* draw_data    = ImGui::GetDrawData();
        const bool  is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

        if (swapchain_texture != nullptr && !is_minimized) {
            // This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
            ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

            // Setup and start a render pass
            SDL_GPUColorTargetInfo target_info = {};
            target_info.texture                = swapchain_texture;
            target_info.clear_color            = SDL_FColor{clear_color.x, clear_color.y, clear_color.z, clear_color.w};
            target_info.load_op                = SDL_GPU_LOADOP_CLEAR;
            target_info.store_op               = SDL_GPU_STOREOP_STORE;
            target_info.mip_level              = 0;
            target_info.layer_or_depth_plane   = 0;
            target_info.cycle                  = false;
            SDL_GPURenderPass* render_pass     = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

            // Render ImGui
            ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

            SDL_EndGPURenderPass(render_pass);
        }

        SDL_SubmitGPUCommandBuffer(command_buffer);
    }

    void App::renderPlatformWindows() {
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

} // namespace VTT::Client
