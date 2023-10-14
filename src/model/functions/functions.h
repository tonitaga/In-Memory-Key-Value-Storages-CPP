#ifndef TRANSACTIONS_LIBRARY_CPP_FUNCTIONS_H
#define TRANSACTIONS_LIBRARY_CPP_FUNCTIONS_H

#include <chrono>

namespace ttl {
    class Functions {
    public:
        template<typename AssociativeContainer>
        static double Execute(int choice, int tests_count, AssociativeContainer storage) {
            if (choice == 1) { // SET
                return getTime([&]() {
                    for (int i = 0; i != tests_count; ++i)
                        storage.insert({i, i});
                });
            }

            if (choice == 2) { // GET
                for (int i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (int i = 0; i != tests_count; ++i)
                        storage[i];
                });
            }

            if (choice == 3) { // EXISTS
                for (int i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (int i = 0; i != tests_count; ++i) {
                        if (storage.find(i) == storage.end())
                            storage[i] = i;
                    }
                });
            }

            if (choice == 4) { // DEL
                for (int i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (int i = 0; i != tests_count; ++i)
                        storage.erase(storage.find(i));
                });
            }

            if (choice == 5) { // UPDATE
                for (int i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (int i = 0; i != tests_count; ++i)
                        storage[i] = i;
                });
            }

            if (choice == 6) { // RENAME
                for (int i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (int i = 0; i != tests_count; ++i) {
                        if (storage.find(i) == storage.end())
                            continue;

                        auto mapped = storage[i];
                        storage.erase(i);
                        storage.insert({i, mapped});
                    }
                });
            }

            if (choice == 7) { // EXISTS
                for (int i = 0; i != tests_count; ++i)
                    storage.insert({i, i});

                return getTime([&]() {
                    for (int i = 0; i != tests_count; ++i) {
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
