#include "view.h"

#include "command_factory.h"
#include "student.h"

#include "map.h"
#include "unordered_map.h"
#include "functions.h"

#include <map>
#include <unordered_map>

namespace ttl {
    void IView::DisplayCommands() {
        std::cout << red   << "---------------------------------" << reset << '\n';
        std::cout << green << "Expected commands:" << reset << '\n';

        std::cout << "> " << green << "SET " << reset << "<key> <surname> <name> <year> <city> <coins>" << '\n';
        std::cout << "If you want to set a life time to key use 'EX' subcommand:" << '\n';
        std::cout << "> " << green << "SET " << reset << "<key> <surname> <name> <year> <city> <coins> EX <time>" << "\n\n";

        std::cout << "> " << green << "GET " << reset << "<key>" << '\n';
        std::cout << "> " << green << "EXISTS " << reset << "<key>" << '\n';
        std::cout << "> " << green << "DEL " << reset << "<key>" << "\n\n";

        std::cout << "> " << green << "UPDATE " << reset << "<key> <surname> <name> <year> <city> <coins>" << '\n';
        std::cout << "If you need to change only some fields of mapped value use '-'" << '\n';
        std::cout << "> " << green << "UPDATE " << reset << "<key> - <name> - - <coins>\n\n";

        std::cout << "> " << green << "KEYS" << reset << '\n';
        std::cout << "> " << green << "RENAME " << reset << "<old_key> <new_key>" << '\n';
        std::cout << "> " << green << "TTL " << reset << "<key>\n\n";

        std::cout << "> " << green << "FIND " << reset << "<key> <surname> <name> <year> <city> <coins>" << std::endl;
        std::cout << "If you want to search by only some fields of mapped value use '-'" << '\n';
        std::cout << "> " << green << "FIND " << reset << "<key> - <name> - - <coins>\n\n";

        std::cout << "> " << green << "SHOWALL" << reset << '\n';
        std::cout << "> " << green << "UPLOAD " << reset << "path/to/file.txt" << '\n';
        std::cout << "> " << green << "EXPORT " << reset << "path/to/file.txt\n\n";

        std::cout << "> " << green << "EXIT" << reset << '\n';
        std::cout << red   << "---------------------------------" << reset << "\n\n";
    }

    void HashTableView::Show() {
        DisplayCommands();

        std::string line;
        ttl::unordered_map<std::string, Student> map;

        while (true) {
            std::getline(std::cin, line, '\n');

            if (line == "EXIT")
                break;

            auto command = CommandFactory::getCommand(line, map);
            if (command == nullptr)
                continue;

            command->Execute(map);
        }
    }

    void RedBlackTreeView::Show() {
        DisplayCommands();

        std::string line;
        ttl::map<std::string, Student> map;

        while (true) {
            std::getline(std::cin, line, '\n');

            if (line == "EXIT")
                break;

            auto command = CommandFactory::getCommand(line, map);
            if (command == nullptr)
                continue;

            command->Execute(map);
        }
    }

    void CompareStoragesView::Show() {
        DisplayCommands();

        int choice, tests_count;

        std::cout << green << "Enter number from list\n> " << reset;
        std::cin >> choice;
        while (choice != 0) {
            std::cout << green << "Enter iterations count [0 - 1'000'000]\n> " << reset;
            std::cin >> tests_count;

            if (tests_count > 1000000) {
                std::cout << red << "Tests count is too big!\n" << green << "Choose less or equal 1'000'000" << reset << "\n\n";
                std::cout << green << "Enter number from list\n> " << reset;
                std::cin >> choice;
                continue;
            }

            double time_ttl_unordered = Functions::Execute(choice, tests_count, ttl::unordered_map<int, int>{});
            double time_std_unordered = Functions::Execute(choice, tests_count, std::unordered_map<int, int>{});
            double time_ttl_map = Functions::Execute(choice, tests_count, ttl::map<int, int>{});
            double time_std_map = Functions::Execute(choice, tests_count, std::map<int, int>{});

            std::cout << std::endl;
            std::cout << red << "ttl::unordered_map (ms): " << reset << time_ttl_unordered << std::endl;
            std::cout << red << "std::unordered_map (ms): " << reset << time_std_unordered << std::endl;
            std::cout << red << "ttl::map (ms):           " << reset << time_ttl_map << std::endl;
            std::cout << red << "std::map (ms):           " << reset << time_std_map << std::endl;
            std::cout << std::endl;

            std::cout << green << "Enter number from list\n> " << reset;
            std::cin >> choice;
        }
    }

    void CompareStoragesView::DisplayCommands() {
        std::cout << red   << "---------------------------------" << reset << '\n';
        std::cout << green << "Expected commands:" << reset << '\n';

        std::cout << "1. " << green << "SET" << reset << "    <compare_times_count>" << std::endl;
        std::cout << "2. " << green << "GET" << reset << "    <compare_times_count>" << std::endl;
        std::cout << "3. " << green << "EXISTS" << reset << " <compare_times_count>" << std::endl;
        std::cout << "4. " << green << "DEL" << reset << "    <compare_times_count>" << std::endl;
        std::cout << "5. " << green << "UPDATE" << reset << " <compare_times_count>" << std::endl;
        std::cout << "6. " << green << "RENAME" << reset << " <compare_times_count>" << std::endl;
        std::cout << "7. " << green << "FIND" << reset << "   <compare_times_count>" << std::endl;
        std::cout << "0. " << green << "EXIT" << reset << std::endl;
        std::cout << red   << "---------------------------------" << reset << "\n\n";
    }

    void GeneratorKeyValueView::Show() {
        DisplayCommands();

        std::string command, path;
        std::cout << "> ";
        std::cin >> command;

        int count;
        while (command != "EXIT") {
            if (command != "GENERATE") {
                std::cout << "> ";
                std::cin >> command;
                continue;
            }

            std::cout << green << "Enter save file path\n" << reset << "> ";
            std::cin >> path;
            std::cout << green << "Enter generation size\n" << reset << "> ";
            std::cin >> count;

            std::ofstream file(path);
            if (file.is_open()) {
                if (count > 100000) {
                    std::cout << red << "Sorry, but your generation size is too big, we cut it to 100000\n" << reset;
                }

                for (int i = 1; i != count; ++i) {
                    file << i << " tonitaga manhunte 21 transactions-a6 0" << '\n';
                }

                std::cout << green << "Filed saved successfully!\n" << reset;
                file.close();
            }

            std::cout << "> ";
            std::cin >> command;
        }
    }

    void GeneratorKeyValueView::DisplayCommands() {
        std::cout << red   << "---------------------------------" << reset << '\n';
        std::cout << green << "Expected commands:" << reset << '\n';
        std::cout << "> " << green << "GENERATE" << reset << std::endl;
        std::cout << "> " << green << "EXIT" << reset << std::endl;
        std::cout << red   << "---------------------------------" << reset << "\n\n";
    }

    std::unique_ptr<IView> getView(int choice) {
        if (choice == 1)
            return std::make_unique<HashTableView>();

        if (choice == 2)
            return std::make_unique<RedBlackTreeView>();

        if (choice == 3)
            return std::make_unique<CompareStoragesView>();

        if (choice == 4)
            return std::make_unique<GeneratorKeyValueView>();

        return nullptr;
    }

    int getStorageChoiceInput() {
        std::cout << green << "> " << reset << "Choose interactive version:" << '\n';
        std::cout << green << "> 1. " << reset << "unordered_map [key-value storage]" << '\n';
        std::cout << green << "> 2. " << reset << "map           [key-value storage]" << '\n';
        std::cout << green << "> 3. " << reset << "compare unordered_map & map" << '\n';
        std::cout << green << "> 4. " << reset << "generate key-value file" << '\n';
        std::cout << red << "> " << reset;

        int choice;
        std::cin >> choice;

        return choice;
    }
}