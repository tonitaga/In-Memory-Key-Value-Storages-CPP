#include "view.h"

int main() {
    using namespace ttl;

    auto view = IView::getView();
    if (view) view->Show();

    return 0;
}
