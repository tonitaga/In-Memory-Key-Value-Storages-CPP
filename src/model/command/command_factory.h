#ifndef TRANSACTIONS_LIBRARY_CPP_COMMAND_FACTORY_H
#define TRANSACTIONS_LIBRARY_CPP_COMMAND_FACTORY_H

#include <iostream>
#include <sstream>
#include <memory>

#include "command.h"

namespace ttl {
    class CommandFactory {
    public:
        template<typename AssociativeContainer>
        static std::unique_ptr<ICommand<AssociativeContainer>> getCommand(const std::string &line, const AssociativeContainer &) {
            std::string command;
            std::stringstream ss(line);

            using key_type = typename AssociativeContainer::key_type;
            using mapped_type = typename AssociativeContainer::mapped_type;

            std::unique_ptr<ICommand<AssociativeContainer>> find_command(nullptr);

            ss >> command;
            if (command == "SET") {
                key_type key; ss >> key;
                mapped_type mapped;
                try {
                    ss >> mapped;
                    find_command = std::make_unique<SetCommand<AssociativeContainer>>(std::move(key), std::move(mapped));
                } catch (std::exception &) {
                    find_command = nullptr;
                }
            } else if (command == "GET") {
                key_type key; ss >> key;
                find_command = std::make_unique<GetCommand<AssociativeContainer>>(std::move(key));
            } else if (command == "EXISTS") {
                key_type key; ss >> key;
                find_command = std::make_unique<ExistsCommand<AssociativeContainer>>(std::move(key));
            } else if (command == "DEL") {
                key_type key; ss >> key;
                find_command = std::make_unique<DeleteCommand<AssociativeContainer>>(std::move(key));
            } else if (command == "UPDATE") {
                key_type key; ss >> key;
                mapped_type mapped;
                try {
                    ss >> mapped;
                    find_command = std::make_unique<UpdateCommand<AssociativeContainer>>(std::move(key),
                                                                                         std::move(mapped));
                } catch (std::exception &) {
                    find_command = nullptr;
                }
            } else if (command == "KEYS") {
                find_command = std::make_unique<KeysCommand<AssociativeContainer>>();
            } else if (command == "RENAME") {
                key_type key1; ss >> key1;
                key_type key2; ss >> key2;
                find_command = std::make_unique<RenameCommand<AssociativeContainer>>(std::move(key1), std::move(key2));
            } else if (command == "TTL") {
                key_type key; ss >> key;
                find_command = std::make_unique<TTLCommand<AssociativeContainer>>(std::move(key));
            } else if (command == "FIND") {
                mapped_type mapped;
                try {
                    ss >> mapped;
                    find_command = std::make_unique<FindCommand<AssociativeContainer>>(std::move(mapped));
                } catch (std::exception &) {
                    find_command = nullptr;
                }
            } else if (command == "SHOWALL") {
                find_command = std::make_unique<ShowAllCommand<AssociativeContainer>>();
            } else if (command == "UPLOAD") {
                std::string path; ss >> path;
                find_command = std::make_unique<UploadCommand<AssociativeContainer>>(std::move(path));
            } else if (command == "EXPORT") {
                std::string path; ss >> path;
                find_command = std::make_unique<ExportCommand<AssociativeContainer>>(std::move(path));
            }

            return find_command;
        }
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_COMMAND_FACTORY_H
