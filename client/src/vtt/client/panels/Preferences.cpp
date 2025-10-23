//
// Created by andy on 10/22/2025.
//

#include "Preferences.hpp"

#include <array>

#include "imgui/imgui.h"

namespace VTT::Client::Panels {
    static std::array TABS = {
        "Test",
        "Test 2"
    };

    void Preferences::Render() {
        ImGui::BeginChild("tabs", ImVec2(0, 0), ImGuiChildFlags_FrameStyle);
        int idx = 0;
        int newIdx = -1;
        for (const auto& tab : TABS) {
            if (ImGui::Selectable(tab, idx == m_CurrentTab)) {
                newIdx = idx;
            }

            idx++;
        }
        if (newIdx != -1) {
            m_CurrentTab = newIdx;
        }
        ImGui::EndChild();
    }
} // namespace VTT::Client::Panels
