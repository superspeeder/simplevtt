//
// Created by andy on 10/21/2025.
//

#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "vtt/client/Panel.hpp"

namespace VTT::Client {

    template <typename T>
    concept panel_has_persistence_data = requires {
        { T::ID } -> std::common_reference_with<uint32_t>;
        { T::NAME } -> std::common_reference_with<std::string_view>;
        { T::PERSISTENT } -> std::common_reference_with<std::bool_constant<true>>;
    };

    class App {
    public:
        App();
        ~App();

        App(const App& other)                = delete;
        App(App&& other) noexcept            = delete;
        App& operator=(const App& other)     = delete;
        App& operator=(App&& other) noexcept = delete;

        void SetupWindow();
        void SetupImGui();
        void SetupImGuiSDLGPU();

        void Run();

        void BeginImGuiFrame();
        void ImGuiFrame();

        void ProcessEvents();
        void RenderAndPresent();

        void MainMenu();

        bool                   IsPanelOpen(const std::shared_ptr<Panel>& panel) const;
        std::shared_ptr<Panel> GetPanel(std::string_view name) const;
        std::shared_ptr<Panel> GetPanel(uint32_t id) const;

        template <std::derived_from<Panel> T, typename... Args>
            requires(std::constructible_from<T, Args...>)
        void OpenPanel(Args&&... args) {
            // Try to reopen the window if we know how
            if constexpr (panel_has_persistence_data<T>) {
                if (ReopenPanel(T::ID)) {
                    return;
                }
            }

            const std::shared_ptr<T> panel = std::make_shared<T>(std::forward<Args>(args)...);
            if (panel->Id() != ~0U) {
                m_PanelsEasyId.insert({panel->Id(), std::static_pointer_cast<Panel>(panel)});
            }
            m_Panels.insert({panel->Name(), std::static_pointer_cast<Panel>(panel)});
        }

        void OpenPanel(const std::shared_ptr<Panel>& panel);

        bool ReopenPanel(uint32_t id);

        void ClosePanel(const std::shared_ptr<Panel>& panel);
        void ClosePanel(std::string_view name);
        void ClosePanel(uint32_t id);

    private:
        SDL_Window*    m_Window;
        SDL_GPUDevice* m_GpuDevice;
        bool           m_ShouldClose = false;

        std::unordered_map<std::string_view, std::shared_ptr<Panel>> m_Panels;
        std::unordered_map<uint32_t, std::shared_ptr<Panel>>         m_PanelsEasyId;

        // Panels which aren't open but have persistence. This is used for good session persistence info.
        // A panel being in here doesn't actually mean that it was closed, just that the system has stored persistence
        // data on it (since pointers are used, the data should be carried over just fine).
        std::unordered_map<uint32_t, std::shared_ptr<Panel>> m_ClosedPanels;

        void renderPass();
        void renderPlatformWindows();

        inline static App* s_Instance = nullptr;
    };

} // namespace VTT::Client
