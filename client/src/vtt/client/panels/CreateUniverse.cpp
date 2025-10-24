//
// Created by andy on 10/23/2025.
//

#include "CreateUniverse.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "vtt/client/App.hpp"

namespace VTT::Client::Panels {
    void CreateUniverse::Render() {
        ImGui::Text("Create New Universe");

        ImGui::InputText("Title", &m_Title);
        ImGui::InputTextMultiline("Description", &m_Description);

        const bool valid = Validate();
        if (!valid) {
            ImGui::BeginDisabled();
        }

        if (ImGui::Button("Create")) {
            // TODO: actually communicate to server about this.
            Close();
        }

        if (!valid) {
            ImGui::EndDisabled();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel")) {
            Close();
        }
    }

    bool CreateUniverse::Validate() const { return m_Title.size() > 0; }
} // namespace VTT::Client::Panels
