#ifndef TRANSACTIONS_LIBRARY_CPP_VIEW_H
#define TRANSACTIONS_LIBRARY_CPP_VIEW_H

#include <memory>

namespace ttl {
    class IView {
    public:
        virtual ~IView() = default;

    public:
        virtual void Show() = 0;

    protected:
        void DisplayCommands();
    };

    class HashTableView final : public IView {
    public:
        ~HashTableView() override = default;

    public:
        void Show() override;
    };

    class RedBlackTreeView final : public IView {
    public:
        ~RedBlackTreeView() override = default;

    public:
        void Show() override;
    };

    std::unique_ptr<IView> getView(int choice);

    int getStorageChoiceInput();
}

#endif //TRANSACTIONS_LIBRARY_CPP_VIEW_H
