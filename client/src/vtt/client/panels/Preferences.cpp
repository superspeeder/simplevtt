//
// Created by andy on 10/22/2025.
//

#include "Preferences.hpp"

#include <array>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace VTT::Client::Panels {
    namespace {
        enum Tabs { AppearanceTab };
    }

    static std::array TABS = {"Appearance", "Test 2"};

    static void centered_text_colored(const ImVec4& color, const char* text) {
        const auto avail = ImGui::GetContentRegionAvail();
        const auto text_size = ImGui::CalcTextSize(text);
        const auto tl = ImVec2((avail.x - text_size.x) / 2.0f, (avail.y - text_size.y) / 2.0f);
        ImGui::SetCursorPos(tl);
        ImGui::TextColored(color, text);
    }

    void Preferences::Render() {
        if (ImGui::BeginTable("preferencestable", 2, ImGuiTableFlags_Resizable)) {
            ImGui::TableNextColumn();
            ImGui::BeginChild("tabs", ImVec2(0, 0), ImGuiChildFlags_FrameStyle);
            int idx    = 0;
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
            ImGui::TableNextColumn();

            ImGui::BeginChild("prefview");
            switch (m_CurrentTab) {
            case AppearanceTab: Appearance(); break;
            default: centered_text_colored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Select a tab to update preferences."); break;
            }
            ImGui::EndChild();

            ImGui::EndTable();
        }
    }

    void Preferences::Appearance() {
        if (ImGui::Button("Classic")) {
            ImGui::StyleColorsClassic();
        }
        ImGui::SameLine();
        if (ImGui::Button("Dark")) {
            ImGui::StyleColorsDark();
        }
        ImGui::SameLine();
        if (ImGui::Button("Light")) {
            ImGui::StyleColorsLight();
        }
    }
} // namespace VTT::Client::Panels
