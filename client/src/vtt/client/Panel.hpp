//
// Created by andy on 10/22/2025.
//

#pragma once
#include <string_view>

#include <memory>

namespace VTT::Client {
    class Panel : public std::enable_shared_from_this<Panel> {
    public:
        explicit Panel(std::string_view name, uint32_t id = ~0U);
        virtual ~Panel() = default;

        virtual void Render() = 0;

        /**
         * @brief Override this to change window creation logic.
         */
        virtual void RenderWindow();

        inline std::string_view Name() const { return m_Name; };
        inline uint32_t         Id() const { return m_Id; }

        /**
         * @brief Override this to prevent the close button from closing the window.
         */
        inline virtual bool IsOpen() const { return m_IsOpen; };

        inline virtual void Reopen() { m_IsOpen = true; };
        inline void Close() { m_IsOpen = false; }

        /**
         * Persistent panels must have a static ID field and a static NAME field to allow for proper recreation.
         */
        inline virtual bool PersistAfterClose() const { return false; };

    private:
        const std::string_view m_Name;
        const uint32_t         m_Id;
        bool                   m_IsOpen = true;
    };

    namespace detail {
        template <size_t N>
        struct StringLiteral {
            constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }

            char value[N];

            constexpr operator std::string_view() const { return value; }
        };
    } // namespace detail


    /**
     * @brief Unique panels have an ID and name that is static
     */
    template <uint32_t id, detail::StringLiteral name>
    class UniquePanel : public Panel {
    public:
        UniquePanel() : Panel(name, id) {}

        static constexpr std::string_view NAME = name;
        static constexpr uint32_t         ID   = id;

    private:
    };
} // namespace VTT::Client

#ifndef VTTC_NO_HELPER_MACROS
#define PANEL_PERSISTENT inline bool PersistAfterClose() const override { return true; }; static constexpr std::bool_constant<true> PERSISTENT{}
#endif

