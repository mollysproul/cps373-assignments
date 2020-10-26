#include "bakery.hpp"
#include <fstream>
#include <iostream>

std::vector<std::string> split_string(std::string &in_string, char delimiter);

void print_bakery(const Bakery& bakery) {
  std::cout << "Employees: " << std::endl;
  for (auto employee : bakery.employees) {
    std::cout << employee << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Items: " << std::endl;
  for (auto item : bakery.items) {
    std::cout << item.name << ", " << item.price << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Orders: " << std::endl;
  for (auto order : bakery.orders) {
    std::cout << order.employee << ": ";
    auto j = 0;
    for (auto item : order.items) {
      std::cout << item.second << " " << item.first;
      j++;
      if (size_t(j) < order.items.size())
        std::cout << ", ";
    }
    std::cout << std::endl;
  }
}

// You shouldn't need to edit this function (unless you want!)
Bakery text_deserializer(std::string file_path) {
  std::ifstream infile(file_path);
  std::string line;
  Bakery bakery;

  while (!infile.eof()) {
    // Employees section
    if (line.compare("@employees") == 0) {
      std::getline(infile, line);
      while (line.size() > 0) {
        bakery.employees.push_back(line);
        std::getline(infile, line);
      }
    }

    // Items section
    if (line.compare("@items") == 0) {
      std::getline(infile, line);
      while (line.size() > 0) {
        auto end = line.find(", ");
        Item item;
        item.name = line.substr(0, end);
        item.price = line.substr(end + 2);
        bakery.items.push_back(item);
        std::getline(infile, line);
      }
    }

    // Orders section
    if (line.compare("@orders") == 0) {
      std::getline(infile, line);
      auto e = bakery.employees;
      while (line.size() > 0) {
        Order order;
        auto end = line.find(": ");
        order.employee = line.substr(0, end);

        // Find all the orders
        auto rest = line.substr(end + 2);
        size_t pos = 0;
        std::string token;
        while ((pos = rest.find(", ")) != std::string::npos) {
          token = rest.substr(0, pos);
          end = token.find(" ");
          auto quantity = token.substr(0, end);
          auto item_name = token.substr(end + 1);
          order.items.push_back(std::make_pair(item_name, quantity));
          rest.erase(0, pos + 2);
        }
        end = rest.find(" ");
        auto quantity = rest.substr(0, end);
        auto item_name = rest.substr(end + 1);
        order.items.push_back(std::make_pair(item_name, quantity));
        bakery.orders.push_back(order);

        // no more lines to read
        if (infile.eof())
          break;

        std::getline(infile, line);
      }
    }

    std::getline(infile, line);
  }

  return bakery;
}

// Implement these 3 functions!
// Hint: use print_bakery to help think about the text serializer
void text_serializer(const Bakery& bakery, std::string file_path) {
    std::ofstream out_file;
    out_file.open(file_path, std::ios::out);
    
    if(out_file.is_open()){
        // print employees
        out_file << "@employees" << std::endl;
        for (auto employee : bakery.employees) {
            out_file << employee << std::endl;
            
        }
        
        // print items
        out_file << std::endl << "@items" << std::endl;
        for (auto item : bakery.items) {
            out_file << item.name << ", " << item.price << std::endl;
        }
        
        // print orders
        out_file << std::endl << "@orders" << std::endl;
        for (auto order : bakery.orders) {
            out_file << order.employee << ": ";
            auto j = 0;
            for (auto item : order.items) {
                out_file << item.second << " " << item.first;
                j++;
                if (size_t(j) < order.items.size())
                    out_file << ", ";
            }
            out_file << std::endl;
        }
        out_file.close();
    }
}

Bakery binary_deserializer(std::string file_path) {
    std::string input_string;
    Bakery bakery;
    std::unordered_map<char, std::string> naming_dict = {};
    char current_char = '0';
    
    // read in file
    std::ifstream infile(file_path, std::ifstream::binary);
    unsigned int size = 0;
    infile.read(reinterpret_cast<char *>(&size), sizeof(size));
    
    input_string.resize(size);
    infile.read(&input_string[0],  input_string.size());
    infile.close();
    
    // now we have the string of input
    // split string by employees, items, orders
    std::vector<std::string> categories = split_string(input_string, '@');
    
    std::string employees_string = categories[0];
    std::string items_string = categories[1];
    std::string orders_string = categories[2];
    
    // go through employees, update naming_dict
    std::vector<std::string> split_employees = split_string(employees_string, ',');
    bakery.employees = split_employees;
    for (auto e : split_employees) {
        naming_dict[current_char] = e;
        current_char += 1;
    }
    
    // go through items, update naming_dict
    std::vector<std::string> split_items = split_string(items_string, ',');
    Item temp_item;
    temp_item.name = "";
    temp_item.price = "";
    for (auto word : split_items) {
        // check if it's the item name or price
        if (word.find('.') == -1) {
            temp_item.name = word;
            naming_dict[current_char] = word;
            current_char += 1;
        }
        else {
            temp_item.price = word;
            bakery.items.push_back(temp_item);
            temp_item.name = "";
            temp_item.price = "";
        }
    }

    // go through orders, use naming_dict
    std::vector<std::string> split_orders = split_string(orders_string, ',');
    
    // orders have string employee // and std::vector<std::pair<std::string, std::string>> items;
    // use order.items.push_back(std::make_pair(item_name, quantity));
    for (auto order : split_orders) {
        Order temp_order;
        //employee is first
        temp_order.employee = naming_dict[order[0]];
        
        // go thorugh each order
        //split order by spaces
        std::vector<std::string> o_split = split_string(order, ' ');
        o_split.erase(o_split.begin());
        
        // items next, name quantity name quantity ...
        std::string temp_name = "";
        for (auto num : o_split) {
            if (temp_name == "") {
                temp_name = naming_dict[num[0]];
            }
            else {
                temp_order.items.push_back(std::make_pair(temp_name, num));
                temp_name = "";
            }
        }
        bakery.orders.push_back(temp_order);
    }
    return bakery;
    
}
 
void binary_serializer(const Bakery& bakery, std::string file_path)
{
    std::string out_string = "@";
    std::unordered_map<std::string, char> naming_dict = {};
    char current_char = '0';
    
    // employees: string
    for (auto employee : bakery.employees) {
        naming_dict[employee] = current_char;
        current_char += 1;
        out_string += employee;
        out_string += ",";
    }
    
    // items: name, price
    out_string += "@";
    for (auto item : bakery.items) {
        naming_dict[item.name] = current_char;
        current_char += 1;
        std::string temp_string = item.name + "," + item.price + ",";
        out_string += temp_string;
    }
    
    // orders: employee, item(first, second)
    // (item_name, quantity)
    out_string += "@";
    for (auto order : bakery.orders) {
        out_string += std::string(1,naming_dict[order.employee]);
        out_string += " ";
        for (auto item : order.items) {
            std::string temp_string = std::string(1,naming_dict[item.first]) + " " + item.second + " ";
            out_string += temp_string;
        }
        out_string += ",";
    }
    
    // write data
    std::ofstream outfile(file_path, std::ofstream::binary);
    unsigned int size = out_string.size();
    outfile.write(reinterpret_cast<char *>(&size), sizeof(size));
    outfile.write(out_string.c_str(), out_string.size());
    outfile.close();
}

 
std::vector<std::string> split_string(std::string &in_string, char delimiter)
{
    std::vector<std::string> out_vec;
    int start = in_string.find_first_not_of(delimiter, 0);
    int end = 0;

    while (start != -1) {
        end = in_string.find(delimiter, start);
        std::string chunk = in_string.substr(start, end - start);
        out_vec.push_back(chunk);
        start = in_string.find_first_not_of(delimiter, end);
    }
    return out_vec;
}

