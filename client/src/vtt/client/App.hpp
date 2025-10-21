//
// Created by andy on 10/21/2025.
//

#pragma once

#include <SDL3/SDL.h>

namespace VTT::Client {

    class App {
    public:
        App();
        ~App();

        App(const App& other)                = delete;
        App(App&& other) noexcept            = delete;
        App& operator=(const App& other)     = delete;
        App& operator=(App&& other) noexcept = delete;

        void Run();

    private:
        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;

        bool m_ShouldClose = false;
    };

} // namespace VTT::Client
