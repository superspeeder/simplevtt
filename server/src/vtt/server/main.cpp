//
// Created by andy on 10/21/2025.
//

#include <vtt/core/core.hpp>

#include <iostream>

#include <boost/asio.hpp>

#include "server.hpp"

int main() {
    std::cout << "Header Version: " << VTT::Core::HeaderVersion << std::endl;
    std::cout << "Library Version: " << VTT::Core::GetLibraryVersion() << std::endl;

    try {
        boost::asio::io_context io_context;
        VTT::Server::Server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
