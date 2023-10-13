#include "view.h"
#include "map.h"
#include "student.h"
#include "unordered_map.h"

int main() {
    using namespace ttl;

    auto view = getView(
            getStorageChoiceInput());

    if (view) view->Show();
    return 0;
}
