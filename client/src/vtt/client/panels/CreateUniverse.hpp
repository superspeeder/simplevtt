//
// Created by andy on 10/23/2025.
//

#pragma once

#include <string>

#include "vtt/client/Panel.hpp"

namespace VTT::Client::Panels {
    namespace ID {
        inline constexpr uint32_t CreateUniverse = 2;
    }

    class CreateUniverse final : public UniquePanel<ID::CreateUniverse, "Create Universe"> {
    public:
        void Render() override;
        bool Validate() const;

    private:
        std::string m_Title;
        std::string m_Description;
    };

} // namespace VTT::Client::Panels
