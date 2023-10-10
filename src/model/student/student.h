#ifndef TRANSACTIONS_LIBRARY_CPP_STUDENT_H
#define TRANSACTIONS_LIBRARY_CPP_STUDENT_H

#include <string>

namespace ttl {
    struct Student {
        std::string surname;
        std::string name;
        int year = -1;
        std::string city;
        int coins = -1;
        int time = -1;
    };

    [[nodiscard]] inline bool operator==(const Student &lhs, const Student &rhs) {
        if (lhs.surname != rhs.surname and rhs.surname != "-")
            return false;
        if (lhs.name != rhs.name and rhs.name != "-")
            return false;
        if (lhs.city != rhs.city and rhs.city != "-")
            return false;
        if (lhs.year != rhs.year and rhs.year != -1)
            return false;
        if (lhs.coins != rhs.coins and rhs.coins != -1)
            return false;
        return true;
    }

    std::istream &operator>>(std::istream &in, Student &rhs) {
        std::string surname, name, city;
        std::string year, coins;
        std::string ex;

        in >> surname >> name >> year >> city >> coins >> ex;
        rhs.surname = std::move(surname);
        rhs.name = std::move(name);
        rhs.city = std::move(city);

        if (year != "-") {
            try {
                rhs.year = std::stoi(year);
            } catch(std::exception &e) {
                std::cout << "Can't convert '" << year << "' to int type" << std::endl;
                throw;
            }
        }

        if (coins != "-") {
            try {
                rhs.coins = std::stoi(coins);
            } catch(std::exception &e) {
                std::cout << "Can't convert '" << coins << "' to int type" << std::endl;
                throw;
            }
        }

        if (ex == "EX") {
            std::string time;
            std::cin >> time;

            try {
                rhs.time = std::stoi(time);
            } catch(std::exception &e) {
                std::cout << "Can't convert '" << time << "' to int type" << std::endl;
                throw;
            }
        }

        return in;
    }

    std::ostream &operator<<(std::ostream &out, const Student &rhs) {
        out << rhs.surname << ' ' << rhs.name << ' ' << rhs.year << ' ' << rhs.city << ' ' << rhs.coins;
        return out;
    }
}

#endif //TRANSACTIONS_LIBRARY_CPP_STUDENT_H
