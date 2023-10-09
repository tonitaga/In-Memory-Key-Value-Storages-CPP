#include <iostream>
#include <map>
#include <unordered_map>
#include <variant>

#include "command_factory.h"

int getChoice() {
    std::cout << "> Choose the storage type:\n";
    std::cout << "> 1 - Hash Table\n";
    std::cout << "> 2 - Red Black Tree\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    return choice;
}

int main() {
    int choice = getChoice();
    if (choice != 1 and choice != 2) {
        std::cerr << "> Invalid choice" << std::endl;
        return EXIT_FAILURE;
    }

    std::unordered_map<std::string, ttl::Student> unordered_map;
    std::map<std::string, ttl::Student> map;

    std::string line;

    while (true) {
        std::getline(std::cin, line);
        if (line == "EXIT")
            break;

        std::cout << "> ";

        if (line.empty())
            continue;

        if (choice == 1) {
            auto command = ttl::CommandFactory::getCommand(line, unordered_map);
            if (command == nullptr) {
                continue;
            }

            command->Execute(unordered_map);
        } else {
            auto command = ttl::CommandFactory::getCommand(line, map);
            if (command == nullptr) {
                continue;
            }

            command->Execute(map);
        }
    }

    return 0;
}
