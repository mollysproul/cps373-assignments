#include <chrono>
#include <iostream>
#include <fstream>
using namespace std;
#include "../include/bakery.hpp"

using namespace std::chrono;
void test_text_deserializer();
void test_text_serializer();
void test_binary_serializer();
void test_binary_deserializer();

int main() {
    test_text_deserializer();
    test_text_serializer();
    test_binary_serializer();
    test_binary_deserializer();
    
    return 0;
}

void test_text_deserializer() {
    //text_deserializer on small.txt
    auto start = high_resolution_clock::now();
    auto bakery = text_deserializer("../data/small.txt");
    auto deser_time = high_resolution_clock::now() - start;
    std::cout   << "small.txt: Text deserializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
    
    //text_deserializer on medium.txt
    start = high_resolution_clock::now();
    bakery = text_deserializer("../data/medium.txt");
    deser_time = high_resolution_clock::now() - start;
    std::cout   << "medium.txt: Text deserializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
    
    //text_deserializer on large.txt
    start = high_resolution_clock::now();
    bakery = text_deserializer("../data/large.txt");
    deser_time = high_resolution_clock::now() - start;
    std::cout   << "large.txt: Text deserializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
}


void test_text_serializer() {
    
    //text_serializer on small.txt
    auto bakery = text_deserializer("../data/small.txt");
    auto start = high_resolution_clock::now();
    text_serializer(bakery, "../output_files/small_serializer.txt");
    auto deser_time = high_resolution_clock::now() - start;
    std::cout   << "small.txt: Text serializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
    
    //text_serializer on medium.txt
    bakery = text_deserializer("../data/medium.txt");
    start = high_resolution_clock::now();
    text_serializer(bakery, "../output_files/medium_serializer.txt");
    deser_time = high_resolution_clock::now() - start;
    std::cout   << "medium.txt: Text serializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
    
    //text_serializer on large.txt
    bakery = text_deserializer("../data/large.txt");
    start = high_resolution_clock::now();
    text_serializer(bakery, "../output_files/large_serializer.txt");
    deser_time = high_resolution_clock::now() - start;
    std::cout   << "large.txt: Text serializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
}

void test_binary_serializer(){
    //text_serializer on small.txt
    auto bakery = text_deserializer("../data/small.txt");
    auto start = high_resolution_clock::now();
    binary_serializer(bakery, "../output_files/small_serializer.bin");
    auto deser_time = high_resolution_clock::now() - start;
    std::cout   << "small.txt: Binary serializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;

    //text_serializer on medium.txt
    bakery = text_deserializer("../data/medium.txt");
    start = high_resolution_clock::now();
    binary_serializer(bakery, "../output_files/medium_serializer.bin");
    deser_time = high_resolution_clock::now() - start;
    std::cout   << "medium.txt: Binary serializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
    
    //text_serializer on large.txt
    bakery = text_deserializer("../data/large.txt");
    start = high_resolution_clock::now();
    binary_serializer(bakery, "../output_files/large_serializer.bin");
    deser_time = high_resolution_clock::now() - start;
    std::cout   << "large.txt: Binary serializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
}

void test_binary_deserializer(){
    //text_deserializer on small.txt
    auto start = high_resolution_clock::now();
    auto bakery = binary_deserializer("../output_files/small_serializer.bin");
    auto deser_time = high_resolution_clock::now() - start;
    std::cout   << "small_serializer.bin: Text deserializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
    
    //text_deserializer on medium.txt
    start = high_resolution_clock::now();
    bakery = binary_deserializer("../output_files/medium_serializer.bin");
    deser_time = high_resolution_clock::now() - start;
    std::cout   << "medium_serializer.bin: Binary deserializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
    
    //text_deserializer on large.txt
    start = high_resolution_clock::now();
    bakery = binary_deserializer("../output_files/large_serializer.bin");
    deser_time = high_resolution_clock::now() - start;
    std::cout   << "large_serializer.bin: Binary deserializer took: "
                << duration_cast<milliseconds>(deser_time).count() << "ms"
                << std::endl
                << std::endl;
}
