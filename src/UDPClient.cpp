#include "UDPClient.h"

#include <boost/bind.hpp>
#include <boost/array.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>

#include "STUN/stunbuilder.h"


using boost::asio::ip::udp;
using boost::asio::ip::address;


//#define SENDIPADDRESS "109.148.196.37"
#define SENDIPADDRESS "stun.aaisp.co.uk"
//#define SENDIPADDRESS "localhost"
#define SEND_UDP_PORT 3478


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

void UDPClient::SendStun()
{
    boost::asio::io_service io_service;

    boost::asio::ip::udp::resolver resolver(io_service);
    boost::asio::ip::udp::resolver::query resolver_query(SENDIPADDRESS,"3478", boost::asio::ip::udp::resolver::query::numeric_service);
    boost::asio::ip::udp::resolver::iterator iter = resolver.resolve(resolver_query);

    udp::socket socket(io_service);
    
    socket.open(udp::v4());
    //socket.bind(udp::endpoint(address::from_string(RECIPADDRESS), SEND_UDP_PORT));
    socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), SEND_UDP_PORT));

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

    std::jthread thr{[&]{ 

        decltype(iter) end;

        for (; iter != end; ++iter) {
            udp::endpoint remote_endpoint = iter->endpoint();

            CStunMessageBuilder builder;
            StunTransactionId transId;

            builder.AddBindingRequestHeader();
            builder.AddRandomTransactionId(&transId);
            unsigned char* msg = builder.GetStream().GetDataPointerUnsafe();
            size_t len = builder.GetStream().GetSize();

            std::cout << "Sending " << len << std::endl;
            std::cout << std::hex << std::setfill('0');
            for(int i=0; i<len; ++i) {
                std::cout << std::setw(2) << static_cast<unsigned>(msg[i]);
            }

            std::cout << std::endl;

            boost::system::error_code err;
            auto sent = socket.send_to(boost::asio::buffer(msg, len), remote_endpoint, 0, err);
            std::cout << "Sent Payload --- " << sent << "\n";
        }
    }};

    std::cout << "IO service Running" << std::endl;
    io_service.run();

    
    socket.close();
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

        CStunMessageBuilder builder;
        StunTransactionId transId;

        builder.AddBindingRequestHeader();
        builder.AddRandomTransactionId(&transId);
        unsigned char* msg = builder.GetStream().GetDataPointerUnsafe();
        size_t len = builder.GetStream().GetSize();

        std::cout << "Sending " << len << std::endl;
        std::cout << std::hex << std::setfill('0');
        for(int i=0; i<len; ++i) {
            std::cout << std::setw(2) << static_cast<unsigned>(msg[i]);
        }

        std::cout << std::endl;


        boost::system::error_code err;
        auto sent = socket.send_to(boost::asio::buffer(msg, len), remote_endpoint, 0, err);    
        std::cout << "Sent Payload --- " << sent << "\n";
        socket.close();
    }

}
