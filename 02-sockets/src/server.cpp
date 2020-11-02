#include <inttypes.h>
#include <asio.hpp>
#include <ctime>
#include <iostream>
#include <string>

#include "bakery.hpp"

using asio::ip::tcp;
int get_response(std::array<uint8_t, 3> buf, Bakery& bakery);

int main() {
    asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 3000));
    
    // Use this bakery to handle queries from the client
    Bakery bakery = text_deserializer("../data/bakery.txt");
    
    while (true) {
        // Wait for client
        std::cout << "Blocked for read" << std::endl;
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::array<uint8_t, 3> buf;
        asio::error_code error;
        size_t len = socket.read_some(asio::buffer(buf), error);
        // Example of error handling
        // if (error != asio::error::eof)
        //   throw asio::system_error(error);
        
        // last step, sends response back
        //std::cout << "Response: " << response << std::endl;
        int response = get_response(buf, bakery);
        std::cout<< "Response: " << response << std::endl;
        
        std::array<uint8_t, 2> return_buf;
        return_buf.fill(0);
        if (response > 255){
            return_buf[0] = 255;
            return_buf[1] = (response - 255);
        }
        else {
            return_buf[0] = response;
        }
        
        std::memcpy(&buf, &return_buf, sizeof(uint16_t));
        asio::write(socket, asio::buffer(buf), error);
    }
    
    return 0;
}

int get_response(std::array<uint8_t, 3> buf, Bakery& bakery)
{
    // to get action, shift 6
    uint8_t m_value = buf[0]>>6;
    
    if (m_value == 0b00) {
        // if M = 00, get total number of orders
        return bakery.orders.size();
    }
    else if (m_value == 0b01) {
        // if M = 01, get number of orders for employee E
        
        uint8_t employee_bits = buf[0] >> 4;
        std::string employee_name;
        
        // find E
        if (employee_bits == 0b0100) {
            employee_name = "Brad";
        }
        else if (employee_bits == 0b0101) {
            employee_name = "Claudia";
        }
        else if (employee_bits == 0b0110) {
            employee_name = "Simone";
        }
        else if (employee_bits == 0b0111) {
            return 0;
        }
        
        int count = 0;
        for (Order temp_order : bakery.orders) {
            if (temp_order.employee == employee_name) {
                count ++;
            }
        }
        return count;
    }
    
    else if (m_value == 0b10){
        // if M = 10, add an order with E, Q1, Q2, ..., Q5 values
        Order temp_order;
        
        uint8_t employee_bits = buf[0] >> 4;
        
        if (employee_bits == 0b1000) {
            temp_order.employee = "Brad";
        }
        else if (employee_bits == 0b1001){
            temp_order.employee = "Claudia";
        }
        else if (employee_bits == 0b1010){
            temp_order.employee = "Simone";
        }
        else if (employee_bits == 0b1011){
            return 0;
        }
        
        // get quantities of all the products
        uint8_t q_1 = buf[0] & 00001111;
        uint8_t q_2 = buf[1] >> 4;
        uint8_t q_3 = buf[1] & 0b00001111;
        uint8_t q_4 = buf[2] >> 4;
        uint8_t q_5 = buf[2] & 0b00001111;
        
        if (not q_1) {
            std::string temp_string = std::to_string(q_1);
            temp_order.items.push_back(std::make_pair("Biscuit", temp_string));
        }
        
        if (not q_2){
            std::string temp_string = std::to_string(q_2);
            temp_order.items.push_back(std::make_pair("Bun", temp_string));
        }
        
        if (not q_3) {
            std::string temp_string = std::to_string(q_3);
            temp_order.items.push_back(std::make_pair("Brownie", temp_string));
        }
        
        if (not q_4) {
            std::string temp_string = std::to_string(q_4);
            temp_order.items.push_back(std::make_pair("White Loaf", temp_string));
        }
        if (not q_5) {
            std::string temp_string = std::to_string(q_5);
            temp_order.items.push_back(std::make_pair("Wheat Loaf", temp_string));
        }
        
        bakery.orders.push_back(temp_order);
        return 0;
    }
    
    return 0;
    
}
