#include "view.h"

#include "map.h"
#include "student.h"
#include "unordered_map.h"

#include <fstream>
#include <iostream>
#include <sstream>

int main() {
//    using namespace ttl;
//
//    auto view = getView(
//            getStorageChoiceInput());
//
//    if (view) view->Show();

    ttl::unordered_map<std::string, ttl::Student> map;
    std::string path = R"(D:\Github\Console\In-Memory-Key-Value-Storages-CPP\cmake-build-debug\data.dat)";
    std::ifstream fin(path);
    if (!fin.is_open()) {
        std::cout << "Open error" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::stringstream ss;
    while (std::getline(fin, line)) {
        ss.clear();
        ss.str(line);

        ttl::Student s;
        std::string key;

        ss >> key >> s;
        std::cout << key << ' ' << s << '\n';
        map.insert({key, s});
    }

    std::cout << map.size() << std::endl;
    fin.close();

    for (const auto &[key, value] : map)
        std::cout << key << ' ' <<  value << '\n';

    return 0;
}
