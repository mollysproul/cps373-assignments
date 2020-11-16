#include <chrono>
#include <fstream>
#include <iostream>

#include "../include/manager.hpp"
#include "../include/router.hpp"

int run_client_server_topology();
int run_ring_topology();
int run_complex_network();

int main() {
    /*
    Manager manager;
    Router r1(1, &manager, {{2, 1000}});
    Router r2(2, &manager, {{1, 1000}, {3, 1000}});
    Router r3(3, &manager, {{2, 1000}});

    r1.set_forwarding_tables({{2, 2}, {3, 2}});
    r2.set_forwarding_tables({{1, 1}, {3, 3}});
    r3.set_forwarding_tables({{1, 2}, {2, 2}});

    manager.routers.push_back(r1);
    manager.routers.push_back(r2);
    manager.routers.push_back(r3);

    // messages
    manager.add_message(3, 1);
    manager.add_message(1, 3);

    // wait until all messages are recieved
    while (!manager.finished()) {
        manager.tick();
    }
    //return 0;
     */
    
    run_client_server_topology();
    run_ring_topology();
    run_complex_network();
    
    return 0;
}

// Client Server topology
int run_client_server_topology() {
    std::cout << "_____________________________________" << std::endl;
    std::cout << "Running Client Server Topology" << std::endl;
    
    // input latency times
    Manager manager;
    Router r1(1, &manager, {{2, 1000},{3, 1000},{4, 1000},{5, 1000}});
    Router r2(2, &manager, {{1, 1000}});
    Router r3(3, &manager, {{1, 1000}});
    Router r4(4, &manager, {{1, 1000}});
    Router r5(5, &manager, {{1, 1000}});
    
    // set up forwarding tables
    r1.set_forwarding_tables({{2, 2}, {3, 3}, {4, 4}, {5, 5}});
    r2.set_forwarding_tables({{1, 1}, {3, 1}, {4, 1}, {5, 1}});
    r3.set_forwarding_tables({{1, 1}, {2, 1}, {4, 1}, {5, 1}});
    r4.set_forwarding_tables({{1, 1}, {2, 1}, {3, 1}, {5, 1}});
    r5.set_forwarding_tables({{1, 1}, {2, 1}, {3, 1}, {4, 1}});
    
    // add each router
    manager.routers.push_back(r1);
    manager.routers.push_back(r2);
    manager.routers.push_back(r3);
    manager.routers.push_back(r4);
    manager.routers.push_back(r5);
    
    // add messages to send
    manager.add_message(2, 1);
    manager.add_message(5, 1);
    manager.add_message(4, 1);
    
    // start timer
    auto start = std::chrono::high_resolution_clock::now();

    // wait until all messages are recieved
    while (!manager.finished()) {
        manager.tick();
    }
    
    // stop timer
    auto deser_time = std::chrono::high_resolution_clock::now() - start;
    std::cout   << "Time took for Client Server Topology: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(deser_time).count() << "ms"
                << std::endl << std::endl;
    return 0;
}

//ring topology
int run_ring_topology() {
    std::cout << "_____________________________________" << std::endl;
    std::cout << "Running Ring Topology" << std::endl;
    
    // input latency times
    Manager manager;
    Router r1(1, &manager, {{2, 100}, {8, 100}});
    Router r2(2, &manager, {{3, 100}, {1, 100}});
    Router r3(3, &manager, {{4, 100}, {2, 100}});
    Router r4(4, &manager, {{5, 100}, {3, 100}});
    Router r5(5, &manager, {{6, 100}, {4, 100}});
    Router r6(6, &manager, {{7, 5000}, {5, 100}});
    Router r7(7, &manager, {{8, 100}, {6, 5000}});
    Router r8(8, &manager, {{1, 100}, {7, 100}});
    
    // set up forwarding tables
    r1.set_forwarding_tables({{2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 8}, {8, 8}});
    r2.set_forwarding_tables({{3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 1}, {8, 1}, {1, 1}});
    r3.set_forwarding_tables({{4, 4}, {5, 4}, {6, 4}, {7, 2}, {8, 2}, {1, 2}, {2, 2}});
    r4.set_forwarding_tables({{5, 5}, {6, 5}, {7, 3}, {8, 3}, {1, 3}, {2, 3}, {3, 3}});
    r5.set_forwarding_tables({{6, 6}, {7, 4}, {8, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}});
    r6.set_forwarding_tables({{7, 5}, {8, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}});
    r7.set_forwarding_tables({{8, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}});
    r8.set_forwarding_tables({{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 7}});
    
    // add each router
    manager.routers.push_back(r1);
    manager.routers.push_back(r2);
    manager.routers.push_back(r3);
    manager.routers.push_back(r4);
    manager.routers.push_back(r5);
    manager.routers.push_back(r6);
    manager.routers.push_back(r7);
    manager.routers.push_back(r8);

    
    // add messages to send
    manager.add_message(1, 3); // 200 ms
    manager.add_message(8, 4); // 400ms
    manager.add_message(6, 7); // 700ms
    
    // start timer
    auto start = std::chrono::high_resolution_clock::now();
    
    // wait until all messages are recieved
    while (!manager.finished()) {
        manager.tick();
    }
    // stop timer
    auto deser_time = std::chrono::high_resolution_clock::now() - start;
    std::cout   << "Time took for Ring Topology: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(deser_time).count() << "ms"
                << std::endl << std::endl;
        
    return 0;
}

//complex network
int run_complex_network() {
    std::cout << "_____________________________________" << std::endl;
    std::cout << "Running Complex Network" << std::endl;

    // input latency times
    Manager manager;
    Router r1(1, &manager, {{2, 300}, {5, 1200}});
    Router r2(2, &manager, {{1, 300}, {3, 800}, {7, 500}});
    Router r3(3, &manager, {{2, 800}, {6, 1500}, {8, 900}, {7, 2000}});
    Router r4(4, &manager, {{7, 300}});
    Router r5(5, &manager, {{1, 1200}, {6, 600}});
    Router r6(6, &manager, {{5, 600}, {3, 1500}, {9, 200}});
    Router r7(7, &manager, {{2, 500}, {3, 2000}, {4, 300}});
    Router r8(8, &manager, {{3, 900}, {9, 400}});
    Router r9(9, &manager, {{6, 200}, {8, 400}});
    
    // set up forwarding tables
    r1.set_forwarding_tables({{2, 2}, {3, 2}, {4, 2}, {5, 5}, {6, 5}, {7, 2}, {8, 2}, {9, 5}});
    r2.set_forwarding_tables({{3, 3}, {4, 7}, {5, 1}, {6, 1}, {7, 7}, {3, 2}, {3, 2}, {1, 2}});
    r3.set_forwarding_tables({{4, 2}, {5, 6}, {6, 6}, {7, 2}, {8, 8}, {9, 8}, {1, 2}, {2, 1}});
    r4.set_forwarding_tables({{5, 7}, {6, 7}, {7, 7}, {8, 7}, {9, 7}, {1, 7}, {2, 7}, {3, 7}});
    r5.set_forwarding_tables({{6, 6}, {7, 1}, {8, 6}, {9, 6}, {1, 1}, {2, 1}, {3, 6}, {4, 1}});
    r6.set_forwarding_tables({{7, 5}, {8, 9}, {9, 9}, {1, 5}, {2, 5}, {3, 3}, {4, 5}, {5, 5}});
    r7.set_forwarding_tables({{8, 2}, {9, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 4}, {5, 2}, {6, 2}});
    r8.set_forwarding_tables({{9, 9}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 9}, {6, 9}, {7, 3}});
    r9.set_forwarding_tables({{1, 6}, {2, 8}, {3, 8}, {4, 8}, {5, 6}, {6, 6}, {7, 8}, {8, 8}});
    
    // add each router
    manager.routers.push_back(r1);
    manager.routers.push_back(r2);
    manager.routers.push_back(r3);
    manager.routers.push_back(r4);
    manager.routers.push_back(r5);
    manager.routers.push_back(r6);
    manager.routers.push_back(r7);
    manager.routers.push_back(r8);
    manager.routers.push_back(r9);
    
    // add messages to send
    manager.add_message(8, 6); // R8 --> R9 --> R6 // 600ms
    manager.add_message(1, 9); //R1 --> R5 --> R6 --> R9 //2000 ms
    manager.add_message(4, 5); // R4 --> R7 --> R2 --> R1 --> R5 //2300
    // start timer
    auto start = std::chrono::high_resolution_clock::now();
    // wait until all messages are recieved
    while (!manager.finished()) {
        manager.tick();
    }
    
    // stop timer
    auto deser_time = std::chrono::high_resolution_clock::now() - start;
    std::cout   << "Time took for Complex Network: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(deser_time).count() << "ms"
                << std::endl << std::endl;
    
    return 0;
}
