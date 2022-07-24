#include "UDPClient.h"

#include <boost/bind.hpp>

#include <boost/array.hpp>

#include <iostream>


using boost::asio::ip::udp;
using boost::asio::ip::address;


//#define SENDIPADDRESS "109.148.196.37"
#define SENDIPADDRESS "stun.l.google.com"
//#define SENDIPADDRESS "localhost"
#define SEND_UDP_PORT 19305


#define RECIPADDRESS "0.0.0.0"

#define UDP_PORT 13251

char recv_buffer[128];
udp::endpoint remote_endpoint;

void UDPClient::wait(udp::socket& socket) {

    socket.async_receive_from(
        boost::asio::buffer(recv_buffer, 128),
        remote_endpoint,
        [this](boost::system::error_code error, size_t bytes_transferred)
        {
            if (error) {
                std::cout << "Receive failed: " << error.message() << "\n";
                return;
            }
            std::cout << "Received: '" << std::string(recv_buffer, recv_buffer + bytes_transferred) << "' (" << error.message() << ")\n";
        }
    );
}

void UDPClient::Receiver()
{
    boost::asio::io_service io_service;
    udp::socket socket(io_service);
    
    socket.open(udp::v4());
    socket.bind(udp::endpoint(address::from_string(RECIPADDRESS), SEND_UDP_PORT));

    wait(socket);

    std::cout << "Receiving\n";
    io_service.run();
    std::cout << "Receiver exit\n";
}

void UDPClient::Send(const std::string& msg) {

    boost::asio::io_service io_service;
    boost::asio::ip::udp::resolver resolver(io_service);
    boost::asio::ip::udp::resolver::query resolver_query(SENDIPADDRESS,"19305", boost::asio::ip::udp::resolver::query::numeric_service);
    boost::asio::ip::udp::resolver::iterator iter = resolver.resolve(resolver_query);

    decltype(iter) end;

    for (; iter != end; ++iter) {
        udp::socket socket(io_service);
        udp::endpoint remote_endpoint = iter->endpoint();
        socket.open(udp::v4());

        boost::system::error_code err;
        auto sent = socket.send_to(boost::asio::buffer(msg), remote_endpoint, 0, err);
        socket.close();
        std::cout << "Sent Payload --- " << sent << "\n";
    }
}
