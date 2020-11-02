#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

int main() {
    asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "3000");
    tcp::socket socket(io_context);
    asio::error_code error;
    
    // • Get the number of orders for E=01 (Claudia)
    // M = 01
    // E = 01
    // Q1 = 0000
    // Q2 = 0000
    // Q3 = 0000
    // Q4 = 0000
    // Q5 = 0000
    std::cout << "Get the number of orders for E=01 (Claudia)" << std::endl;
    std::array<uint8_t, 3> buf = {0b01010000, 0b00000000, 0b00000000};
    
    asio::connect(socket, endpoints);
    asio::write(socket, asio::buffer(buf), error);
    size_t len = socket.read_some(asio::buffer(buf), error);
    uint16_t response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
    std::cout << "Response: " << response << std::endl;
    
    // • Add an order: E=01, Q1=2, Q2=5, Q3=0, Q4=1, Q5=0
    // M = 10
    // E = 01
    // Q1 = 0010
    // Q2 = 0101
    // Q3 = 0000
    // Q4 = 0001
    // Q5 = 0000
    std::cout << "Add an order: E=01, Q1=2, Q2=5, Q3=0, Q4=1, Q5=0" << std::endl;
    buf = {0b10010010, 0b01010000, 0b00010000};
    
    asio::connect(socket, endpoints);
    asio::write(socket, asio::buffer(buf), error);
    len = socket.read_some(asio::buffer(buf), error);
    response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
    std::cout << "Response: " << response << std::endl;
    
    // • Get the number of orders for E=01 (should have increased by one!)
    // M = 01
    // E = 01
    // Q1 = 0000
    // Q2 = 0000
    // Q3 = 0000
    // Q4 = 0000
    // Q5 = 0000
    std::cout << "Get the number of orders for E=01 (should have increased by one!)" << std::endl;
    buf = {0b01010000, 0b00000000, 0b00000000};
    
    asio::connect(socket, endpoints);
    asio::write(socket, asio::buffer(buf), error);
    len = socket.read_some(asio::buffer(buf), error);
    response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
    std::cout << "Response: " << response << std::endl;
    
    // • Get the total number of orders
    // M = 00
    // E = 00
    // Q1 = 0000
    // Q2 = 0000
    // Q3 = 0000
    // Q4 = 0000
    // Q5 = 0000
    std::cout << "Get the total number of orders" << std::endl;
    buf = {0b00000000, 0b00000000, 0b00000000};
    
    asio::connect(socket, endpoints);
    asio::write(socket, asio::buffer(buf), error);
    len = socket.read_some(asio::buffer(buf), error);
    response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
    std::cout << "Response: " << response << std::endl;
    
    return 0;
}
