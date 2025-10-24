//
// Created by andy on 10/23/2025.
//

#include "server.hpp"

namespace VTT::Server {
    Server::Server(boost::asio::io_context& io_context) :
        _io_context(io_context), _acceptor(io_context, tcp::endpoint(tcp::v6(), 30555)) {
        start_accept();
    }

    void Server::start_accept() {
        Connection::pointer new_connection = Connection::create(_io_context);
    }
} // namespace VTT::Server
