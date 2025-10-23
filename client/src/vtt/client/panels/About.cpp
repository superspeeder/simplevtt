//
// Created by andy on 10/22/2025.
//

#include "About.hpp"

#include "imgui/imgui.h"
#include "vtt/core/core.hpp"

namespace VTT::Client::Panels {
    void About::Render() {
        ImGui::Text("SimpleVTT Version: %llu", Core::GetLibraryVersion());
    }
} // namespace VTT::Client
