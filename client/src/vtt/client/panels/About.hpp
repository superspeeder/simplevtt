//
// Created by andy on 10/22/2025.
//

#pragma once
#include "vtt/client/Panel.hpp"

namespace VTT::Client::Panels {
    namespace ID {
        inline constexpr uint32_t About = 1;
    }

    class About final : public UniquePanel<ID::About, "About"> {
    public:
        void Render() override;
    };

} // namespace VTT::Client
