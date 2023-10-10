#include "view.h"

int main() {
    using namespace ttl;

    auto view = getView(
            getStorageChoiceInput());

    view->Start();
    return 0;
}
