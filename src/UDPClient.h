#pragma once

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <string>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

class UDPClient {
public: 
    void Receiver();
    void Send(const std::string& msg);

private:
    void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);
    void wait(boost::asio::ip::udp::socket& socket);


};
