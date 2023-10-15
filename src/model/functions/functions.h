#ifndef TRANSACTIONS_LIBRARY_CPP_FUNCTIONS_H
#define TRANSACTIONS_LIBRARY_CPP_FUNCTIONS_H

#include <chrono>

namespace ttl {
    class Functions {
    public:
        template<typename AssociativeContainer>
        static double Execute(std::size_t choice, std::size_t tests_count, AssociativeContainer storage) {
            using key_type = typename AssociativeContainer::key_type;
            using mapped_type = typename AssociativeContainer::mapped_type;

            if constexpr (std::is_same_v<AssociativeContainer, unordered_map<key_type, mapped_type>>)
                storage.reserve(tests_count);

            if (choice == 1) { // SET
                return getTime([&]() {
                    for (std::size_t i = 0; i != tests_count; ++i)
                        storage.insert({i, i});
                });
            }

            if (choice == 2) { // GET
                for (std::size_t i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (std::size_t i = 0; i != tests_count; ++i)
                        storage[i];
                });
            }

            if (choice == 3) { // EXISTS
                for (std::size_t i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (std::size_t i = 0; i != tests_count; ++i) {
                        if (storage.find(i) == storage.end())
                            storage[i] = i;
                    }
                });
            }

            if (choice == 4) { // DEL
                for (std::size_t i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (std::size_t i = 0; i != tests_count; ++i)
                        storage.erase(storage.find(i));
                });
            }

            if (choice == 5) { // UPDATE
                for (std::size_t i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (std::size_t i = 0; i != tests_count; ++i)
                        storage[i] = i;
                });
            }

            if (choice == 6) { // RENAME
                for (std::size_t i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (std::size_t i = 0; i != tests_count; ++i) {
                        if (storage.find(i) == storage.end())
                            continue;

                        auto mapped = storage[i];
                        storage.erase(i);
                        storage.insert({i, mapped});
                    }
                });
            }

            if (choice == 7) { // EXISTS
                for (std::size_t i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (std::size_t i = 0; i != tests_count; ++i) {
                        if (storage.find(i) == storage.end())
                            storage[i] = i;
                    }
                });
            }

            return 0;
        }

    private:
        template<typename Functor>
        static double getTime(Functor functor) {
            using namespace std::chrono;
            auto begin = high_resolution_clock::now();
            functor();
            auto end = high_resolution_clock::now();
            return duration_cast<microseconds>(end - begin).count() / 1000.0;
        }
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_FUNCTIONS_H
