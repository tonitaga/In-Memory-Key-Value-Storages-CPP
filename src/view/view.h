#ifndef TRANSACTIONS_LIBRARY_CPP_VIEW_H
#define TRANSACTIONS_LIBRARY_CPP_VIEW_H

#include <memory>
#include <functional>

namespace ttl {
    class IView {
    public:
        virtual ~IView() = default;

    public:
        virtual void Show() = 0;

        static std::unique_ptr<IView> getView();

    protected:
        virtual void DisplayCommands();
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

    class CompareStoragesView final : public IView {
    public:
        ~CompareStoragesView() override = default;

    public:
        void Show() override;

    private:
        void DisplayCommands() override;
    };

    class GeneratorKeyValueView final : public IView {
    public:
        ~GeneratorKeyValueView() override = default;

    public:
        void Show() override;

    private:
        void DisplayCommands() override;
    };

    std::unique_ptr<IView> getView(int choice);

    int getStorageChoiceInput();
}

#endif //TRANSACTIONS_LIBRARY_CPP_VIEW_H
