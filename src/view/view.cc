#include "view.h"

#include "command_factory.h"
#include "student.h"

#include "map.h"
#include "unordered_map.h"

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
        ttl::Map<std::string, Student> map;

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

        enum CompareCommands {
            kExit,
            kSetCompare,
            kGetCompare,
            kExistsCompare,
            kDelCompare,
            kUpdateCompare,
            kRenameCompare,
            kFindCompare,
        };

        CompareCommands compare_command; int tmp;
        std::cin >> tmp;
        compare_command = static_cast<CompareCommands>(tmp);

        ttl::unordered_map<std::string, Student> map_unordered;
        ttl::Map<std::string, Student> map_rbtree;

        while (compare_command != kExit) {
            /* ... */
            std::cin >> tmp;
            compare_command = static_cast<CompareCommands>(tmp);
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
    }

    std::unique_ptr<IView> getView(int choice) {
        if (choice == 1)
            return std::make_unique<HashTableView>();

        if (choice == 2)
            return std::make_unique<RedBlackTreeView>();

        if (choice == 3)
            return std::make_unique<CompareStoragesView>();

        return nullptr;
    }

    int getStorageChoiceInput() {
        std::cout << "> Choose the storage type:" << '\n';
        std::cout << "> 1) Hash Table Storage" << '\n';
        std::cout << "> 2) Red Black Tree Storage" << '\n';
        std::cout << "> 3) Compare Storages" << '\n';
        std::cout << "> ";

        int choice;
        std::cin >> choice;

        return choice;
    }
}