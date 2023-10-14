//#include "view.h"
//#include "map.h"
//#include "student.h"
#include "unordered_map.h"

#include <unordered_map>

#include <chrono>

void speed_test() {
    using namespace std::chrono;
    int tests_count = 5;

    double ttl_ = 0, std_ = 0;
    for (int k = 0; k != tests_count; ++k) {
        {
            ttl::unordered_map<int, int> ttlmap;
            auto begin = system_clock::now();
            for (int i = 0; i != 1000000; i++) {
                ttlmap.insert({i, i});
                if (ttlmap.find(i) != ttlmap.end())
                    ttlmap.erase(ttlmap.find(i)->first);
            }
            auto end = system_clock::now();
            ttl_ += duration_cast<nanoseconds>(end - begin).count() / 1'000'000.0;
        }
        {
            std::unordered_map<int, int> stdmap;
            auto begin = system_clock::now();
            for (int i = 0; i != 1000000; i++) {
                stdmap.insert({i, i});
                if (stdmap.find(i) != stdmap.end())
                    stdmap.erase(stdmap.find(i)->first);
            }

            auto end = system_clock::now();
            std_ += duration_cast<nanoseconds>(end - begin).count() / 1'000'000.0;
        }
    }

    std::cout << "ttl::unordered_map: " << ttl_ / tests_count << '\n';
    std::cout << "std::unordered_map: " << std_ / tests_count << '\n';
}


int main() {
//    using namespace ttl;
//
//    auto view = getView(
//            getStorageChoiceInput());
//
//    if (view) view->Show();

    speed_test();

    return 0;
}
