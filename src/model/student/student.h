#ifndef TRANSACTIONS_LIBRARY_CPP_STUDENT_H
#define TRANSACTIONS_LIBRARY_CPP_STUDENT_H

#include <string>
#include <chrono>

namespace ttl {
    using time_point_t = decltype(std::chrono::system_clock::now());

    struct Student {
        std::string surname;
        std::string name;
        int year = -1;
        std::string city;
        int coins = -1;
        int time = -1;
        time_point_t life_begin;

        Student &operator=(const Student &rhs) {
            surname = rhs.surname == "-" ? surname : rhs.surname;
            name = rhs.name == "-" ? name : rhs.name;
            year = rhs.year == -1 ? year : rhs.year;
            city = rhs.city == "-" ? city : rhs.city;
            coins = rhs.coins == -1 ? coins : rhs.coins;
            time = rhs.time == -1 ? time : rhs.time;

            return *this;
        }
    };
}

[[nodiscard]] bool operator==(const ttl::Student &lhs, const ttl::Student &rhs);

std::istream &operator>>(std::istream &in, ttl::Student &rhs);
std::ostream &operator<<(std::ostream &out, const ttl::Student &rhs);

#endif //TRANSACTIONS_LIBRARY_CPP_STUDENT_H
