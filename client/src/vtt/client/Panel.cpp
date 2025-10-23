//
// Created by andy on 10/22/2025.
//

#include "Panel.hpp"

#include "imgui/imgui.h"

namespace VTT::Client {
    Panel::Panel(const std::string_view name, const uint32_t id) : m_Name(name), m_Id(id) {}

    void Panel::RenderWindow() {
        ImGui::Begin(m_Name.data(), &m_IsOpen);
        Render();
        ImGui::End();
    }
} // namespace VTT::Client
