#include <zmq.hpp>
#include <string>
#include <iostream>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);
    
    std::cout << "Connecting to trading engine..." << std::endl;
    socket.connect("tcp://localhost:5555");

    std::cout << "Connected. Type a command and press Enter." << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  ADD [INST] [SIDE] [PX] [QTY] {DISP_QTY}" << std::endl;
    std::cout << "  CANCEL [ORDER_ID] [INST]" << std::endl;
    std::cout << "  DEPTH [INST]" << std::endl;
    std::cout << "  POSITIONS" << std::endl;
    std::cout << "  INSTRUMENTS" << std::endl;
    std::cout << "  EXIT" << std::endl;
    std::cout << "------------------------------------------" << std::endl;


    while (true) {
        std::cout << "\n> " << std::flush;
        std::string line;
        if (!std::getline(std::cin, line) || line == "EXIT") {
            break;
        }

        if (line.empty()) {
            continue;
        }

        // Send the command to the engine
        socket.send(zmq::buffer(line), zmq::send_flags::none);

        // Wait for the reply
        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);
        
        // Print the engine's reply
        std::cout << "--- Engine Reply ---\n" << reply.to_string() << "--------------------" << std::endl;
    }

    std::cout << "Client shutting down." << std::endl;
    return 0;
}