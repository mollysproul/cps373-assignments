#include <chrono>
#include <iostream>
#include "../include/bakery.hpp"
using namespace std::chrono;

int main() {
  auto start = high_resolution_clock::now();
  auto bakery = text_deserializer("../data/small.txt");
  auto deser_time = high_resolution_clock::now() - start;
  std::cout << "Text deserializer took: "
            << duration_cast<milliseconds>(deser_time).count() << "ms"
            << std::endl
            << std::endl;
  print_bakery(bakery);

  return 0;
}