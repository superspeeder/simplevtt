//
// Created by andy on 10/23/2025.
//

#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace VTT::Server {

    class Server {
    public:
        explicit Server(boost::asio::io_context& io_context);

        Server(const Server& other)                = delete;
        Server(Server&& other) noexcept            = delete;
        Server& operator=(const Server& other)     = delete;
        Server& operator=(Server&& other) noexcept = delete;

        void start_accept();
    private:
        boost::asio::io_context& _io_context;
        tcp::acceptor _acceptor;

    };

} // namespace VTT::Server
