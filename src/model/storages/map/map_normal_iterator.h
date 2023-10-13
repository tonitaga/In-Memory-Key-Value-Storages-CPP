#ifndef TRANSACTIONS_LIBRARY_CPP_MAP_NORMAL_ITERATOR_H
#define TRANSACTIONS_LIBRARY_CPP_MAP_NORMAL_ITERATOR_H

namespace ttl {
    template <typename Node>
    class MapNormalIterator {
    public:
        using node_type = Node;
        using node_pointer = node_type *;
        using difference_type = std::ptrdiff_t;
        using value_type = typename Node::value_type;
        using reference = value_type &;
        using pointer = value_type *;

    public:
        explicit MapNormalIterator(node_pointer current) noexcept : current_(current) {};

    private:
        node_pointer current_;

        bool is_nil(const node_pointer &p) { return p->right == p; }

    public:
        reference operator*() { return current_->key_value; }
        const pointer operator->() { return &(current_->key_value); }

        MapNormalIterator &operator++() {
            if (is_nil(current_)) {
                current_ = current_->parent;
                while (!is_nil(current_->left))
                    current_ = current_->left;
            } else if (!is_nil(current_->right)) {
                current_ = current_->right;
                while (!is_nil(current_->left))
                    current_ = current_->left;
            } else {
                while (!is_nil(current_) and current_->parent->left != current_)
                    current_ = current_->parent;
                if (!is_nil(current_))
                    current_ = current_->parent;
            }
            return *this;
        }

        MapNormalIterator operator++(int) {
            MapNormalIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        MapNormalIterator &operator--() {
            if (is_nil(current_)) {
                current_ = current_->parent;
                while (!is_nil(current_->right))
                    current_ = current_->right;
            } else if (!is_nil(current_->left)) {
                current_ = current_->left;
                while (!is_nil(current_->right))
                    current_ = current_->right;
            } else {
                while (!is_nil(current_) and current_->parent->right != current_)
                    current_ = current_->parent;
                if (!is_nil(current_))
                    current_ = current_->parent;
            }
            return *this;
        }

        node_pointer node() const noexcept { return current_; }

        MapNormalIterator operator--(int) {
            MapNormalIterator tmp(*this);
            --(*this);
            return tmp;
        }
    };

    template <typename Node>
    [[nodiscard]] inline bool operator==(const MapNormalIterator<Node> &lhs, const MapNormalIterator<Node> &rhs) {
        return lhs.node() == rhs.node();
    }

    template <typename Node>
    [[nodiscard]] inline bool operator!=(const MapNormalIterator<Node> &lhs, const MapNormalIterator<Node> &rhs) {
        return lhs.node() != rhs.node();
    }
}

#endif //TRANSACTIONS_LIBRARY_CPP_MAP_NORMAL_ITERATOR_H
