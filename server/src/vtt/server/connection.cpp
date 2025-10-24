//
// Created by andy on 10/23/2025.
//

#include "connection.hpp"

namespace VTT::Server {
    void Connection::start() {
        // boost::asio::async_write()
    }


    Connection::Connection(boost::asio::io_context& io_context) : _socket(io_context) {
    }

    void Connection::handle_write(const boost::system::error_code& error, size_t bytes_transferred) {}
} // namespace VTT::Server
