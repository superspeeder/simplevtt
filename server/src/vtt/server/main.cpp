//
// Created by andy on 10/21/2025.
//

#include <vtt/core/core.hpp>

#include <iostream>

int main() {
    std::cout << "Header Version: " << VTT::Core::HeaderVersion << std::endl;
    std::cout << "Library Version: " << VTT::Core::GetLibraryVersion() << std::endl;
    return 0;
}
