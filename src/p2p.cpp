#include <iostream>

#include "UDPClient.h"


enum MODES{RECEIVER, SENDER};


int main(int argc, char* argv[])
{
    MODES mode = argc > 1 ? MODES::RECEIVER : MODES::SENDER;
    if(mode == MODES::RECEIVER)
        std::cout << "Mode is Receiver'" << std::endl;
    else 
        std::cout << "Mode is Sender'" << std::endl;

    if (mode == MODES::RECEIVER) {
        UDPClient client;
        client.Receiver();
    }
    else {
        std::string msg;
        UDPClient client;
        while (msg.compare("quit")) {
            std::cout << "Enter a message: ";
            std::cin >> msg;
            client.Send(msg);
        }
        
    }
    
}