#include <asio.hpp>
#include <iostream>
#include "../include/sender.hpp"
#include <fstream>
#include <unistd.h>
#include <chrono>

using asio::ip::tcp;
int main() {
    auto start = std::chrono::high_resolution_clock::now();
    
    asio::io_context io_context;
    Sender sender(io_context, "127.0.0.1", "3000");
    std::ofstream out_file;
    out_file.open("../out_poem.txt");

    // An explanation of the API
    // data_ready(): returns true if a message is available, otherwise false
    // get_msg(): gets an available message
    // request_msg(id): requests a message at id [0, NUM_MSGS)
    
    std::vector<std::string> poem_dict(853, "");
     
    int max = 853;
    int current_index = 0;
    
    while (current_index < max) {
        // check if the current_index is all set
        if (poem_dict[current_index] != ""){
            //std::cout<<current_index<<std::endl;
            out_file << poem_dict[current_index];
            poem_dict[current_index] = "";
            ++current_index;
        }
        else {
            // request next 10 messages, assume > 1 will be dropped
            for (int i = 0; i < 10; i++) {
                sender.request_msg(current_index + i);
            }
            
            // wait 1 second
            usleep(1000000);
            
            std::vector<Msg> temp_vector(10);
            //try to get 10 messages, fine if not
            for (int i = 0; i < 10; i++) {
                if (sender.data_ready()) {
                    auto msg = sender.get_msg();
                    if (msg.msg_id == current_index) {
                        out_file << std::string(msg.data.data(), CHUNK_SIZE);
                        ++current_index;
                    }
                    else if (poem_dict[msg.msg_id] == "") {
                        poem_dict[msg.msg_id] = std::string(msg.data.data(), CHUNK_SIZE);
                    }
                }
            }
        }
    }
    out_file.close();
    
    auto deser_time = std::chrono::high_resolution_clock::now() - start;
    std::cout   << "total time took: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
    return 0;
}
