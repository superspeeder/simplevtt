//
// Created by andy on 10/21/2025.
//

#include <vtt/core/core.hpp>

#include <iostream>

#include "vtt/client/App.hpp"

int main() {
    std::cout << "Header Version: " << VTT::Core::HeaderVersion << std::endl;
    std::cout << "Library Version: " << VTT::Core::GetLibraryVersion() << std::endl;

    VTT::Client::App app{};
    app.Run();
    return 0;
}
