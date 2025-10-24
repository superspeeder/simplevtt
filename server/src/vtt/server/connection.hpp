//
// Created by andy on 10/23/2025.
//

#pragma once
#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;

namespace VTT::Server {
    class Connection : public std::enable_shared_from_this<Connection> {
    public:
        typedef std::shared_ptr<Connection> pointer;
        inline static pointer create(boost::asio::io_context& io_context) {
            return pointer(new Connection(io_context));
        };

        inline tcp::socket& socket() {
            return _socket;
        }

        void start();

    private:
        explicit Connection(boost::asio::io_context& io_context);

        void handle_write(const boost::system::error_code& error, size_t bytes_transferred);

        tcp::socket _socket;
    };
} // namespace VTT::Server
