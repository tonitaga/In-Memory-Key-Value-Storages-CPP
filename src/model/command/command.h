#ifndef TRANSACTIONS_LIBRARY_CPP_COMMAND_H
#define TRANSACTIONS_LIBRARY_CPP_COMMAND_H

#include <fstream>
#include <iostream>
#include <utility>

#include "student.h"
#include "termcolor.h"

using namespace termcolor;

namespace ttl {
    template <typename AssociativeContainer>
    class ICommand {
    public:
        using key_type = typename AssociativeContainer::key_type;
        using mapped_type = typename AssociativeContainer::mapped_type;

        virtual ~ICommand() = default;
        virtual void Execute(AssociativeContainer &storage) = 0;
    };

    template <typename AssociativeContainer>
    class SetCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;
        
        SetCommand(key_type &&key, mapped_type &&mapped)
            : key_(std::move(key)), mapped_(std::move(mapped)) {}

        void Execute(AssociativeContainer &storage) override {
            storage[key_] = std::move(mapped_);
            std::cout << green << "> OK" << reset << std::endl;
        }

    private:
        key_type key_;
        mapped_type mapped_;
    };

    template <typename AssociativeContainer>
    class GetCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;
        
        explicit GetCommand(key_type &&key)
            : key_(std::move(key)) {}

        void Execute(AssociativeContainer &storage) override {
            if (storage.find(key_) == storage.end()) {
                std::cout << red << "> (null)" << reset << std::endl;
                return;
            }

            std::cout << green << "> " << storage[key_] << reset << std::endl;
        }

    private:
        key_type key_;
    };

    template <typename AssociativeContainer>
    class ExistsCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;
        
        explicit ExistsCommand(key_type &&key)
            : key_(std::move(key)) {}

        void Execute(AssociativeContainer &storage) override {
            if (storage.find(key_) == storage.end()) {
                std::cout << red << "> false" << reset << std::endl;
                return;
            }

            std::cout << green << "> true" << reset << std::endl;
        }

    private:
        key_type key_;
    };

    template <typename AssociativeContainer>
    class DeleteCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;
        
        explicit DeleteCommand(key_type &&key)
            : key_(std::move(key)) {}

        void Execute(AssociativeContainer &storage) override {
            if (storage.find(key_) == storage.end()) {
                std::cout << red << "> false" << reset << std::endl;
                return;
            }
            storage.erase(storage.find(key_));
            std::cout << green << "> true" << reset << std::endl;
        }

    private:
        key_type key_;
    };

    template <typename AssociativeContainer>
    class UpdateCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;

        UpdateCommand(key_type &&key, mapped_type &&mapped)
            : key_(std::move(key)), mapped_(std::move(mapped)) {
        }

        void Execute(AssociativeContainer &storage) override {
            if (storage.find(key_) == storage.end()) {
                std::cout << red << "> (null)" << reset << std::endl;
                return;
            }

            storage[key_] = std::move(mapped_);
            std::cout << green << "> OK" << reset << std::endl;
        }

    private:
        key_type key_;
        mapped_type mapped_;
    };

    template <typename AssociativeContainer>
    class KeysCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;
        
        void Execute(AssociativeContainer &storage) override {
            if (storage.empty()) {
                std::cout << red << "> (null)" << reset << std::endl;
                return;
            }

            for (const auto &[key, mapped] : storage)
                std::cout << green << "> " << key << std::endl;
            std::cout << reset;
        }
    };

    template <typename AssociativeContainer>
    class RenameCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;
        
        RenameCommand(key_type &&key1, key_type &&key2)
            : key1_(std::move(key1)), key2_(std::move(key2)) {}

        void Execute(AssociativeContainer &storage) override {
            if (storage.find(key1_) == storage.end()) {
                std::cout << red << "> key '" << key1_ << "' doesn't exists in storage" << reset << std::endl;
                return;
            }

            mapped_type saved = std::move(storage[key1_]);
            storage.erase(storage.find(key1_));
            storage[key2_] = std::move(saved);
            std::cout << green << "> OK" << reset << std::endl;
        }

    private:
        key_type key1_;
        key_type key2_;
    };

    template <typename AssociativeContainer>
    class TTLCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;
        
        explicit TTLCommand(key_type &&key)
            : key_(std::move(key)) {}

        void Execute(AssociativeContainer &storage) override {
            std::cout << "TTL command executed!" << std::endl;
        }

    private:
        key_type key_;
    };

    template <typename AssociativeContainer>
    class FindCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;
        
        explicit FindCommand(mapped_type &&mapped)
            : mapped_(std::move(mapped)) {}

        void Execute(AssociativeContainer &storage) override {
            if (storage.empty()) {
                std::cout << red << "> (null)" << reset << std::endl;
                return;
            }

            for (const auto &[key, mapped] : storage) {
                if (mapped == mapped_)
                    std::cout << green << "> " << key << reset << std::endl;
            }
        }

    private:
        mapped_type mapped_;
    };

    template <typename AssociativeContainer>
    class ShowAllCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;

        void Execute(AssociativeContainer &storage) override {
            if (storage.empty()) {
                std::cout << red << "> (null)" << reset << std::endl;
                return;
            }

            for (const auto &[key, mapped] : storage)
                std::cout << green << "> " << mapped << std::endl;
            std::cout << reset;
        }
    };

    template <typename AssociativeContainer>
    class UploadCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;
        
        explicit UploadCommand(std::string &&path)
            : path_(std::move(path)) {}

        void Execute(AssociativeContainer &storage) override {
            std::ifstream file(path_);
            if (!file.is_open()) {
                std::cout << red << "> Can't create or open the file by path '" << path_ << "'" << reset << std::endl;
                return;
            }

            key_type key;
            mapped_type mapped;
            std::string line;
            std::stringstream ss;

            std::size_t read_count = 0;
            while (std::getline(file, line)) {
                if (line.empty())
                    continue;

                ss.clear();
                ss.str(line);
                ss >> key >> mapped;

                storage[std::move(key)] = std::move(mapped);
                ++read_count;
            }

            file.close();
            std::cout << green << "> OK " << read_count << reset << std::endl;
        }

    private:
        std::string path_;
    };

    template <typename AssociativeContainer>
    class ExportCommand : public ICommand<AssociativeContainer> {
    public:
        using typename ICommand<AssociativeContainer>::key_type;
        using typename ICommand<AssociativeContainer>::mapped_type;
        
        explicit ExportCommand(std::string &&path)
                : path_(std::move(path)) {}

        void Execute(AssociativeContainer &storage) override {
            std::ofstream file(path_);
            if (!file.is_open()) {
                std::cout << red << "> Can't create or open the file by path '" << path_ << "'" << reset << std::endl;
                return;
            }

            std::size_t write_count = 0;
            for (const auto &[key, mapped] : storage) {
                file << key << ' ' << mapped << '\n';
                ++write_count;
            }

            file.close();
            std::cout << green << "> OK " << write_count << reset << std::endl;
        }

    private:
        std::string path_;
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_COMMAND_H
