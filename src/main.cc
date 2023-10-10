#include "view.h"

int main() {
    using namespace ttl;

    auto view = getView(
            getStorageChoiceInput());

    if (view) view->Show();

    return 0;
}
