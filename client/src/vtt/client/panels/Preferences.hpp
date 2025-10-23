//
// Created by andy on 10/22/2025.
//

#pragma once
#include <cstdint>

#include "vtt/client/Panel.hpp"

namespace VTT::Client::Panels {

    namespace ID {
        inline constexpr uint32_t Preferences = 2;
    }

    class Preferences final : public PersistentPanel<ID::Preferences, "Preferences"> {
    public:
        Preferences() {}
        void Render() override;

    private:
        int m_CurrentTab = 0;

    };

} // namespace VTT::Client::Panels
