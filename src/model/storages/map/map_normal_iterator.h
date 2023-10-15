#ifndef TRANSACTIONS_LIBRARY_CPP_map_NORMAL_ITERATOR_H
#define TRANSACTIONS_LIBRARY_CPP_map_NORMAL_ITERATOR_H

#include <iterator>

namespace ttl {
    template <typename Node>
    class map_normal_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using node_type = Node;
        using node_pointer = node_type *;
        using value_type = typename Node::value_type;
        using reference = std::conditional_t<std::is_const_v<node_type>, typename Node::const_reference, typename Node::reference>;
        using pointer = std::conditional_t<std::is_const_v<node_type>, typename Node::const_pointer, typename Node::pointer>;

    public:
        map_normal_iterator(node_pointer current, node_pointer null, node_pointer root) noexcept
            : current_(current), null_(null), root_(root) {};

    private:
        node_pointer current_;
        node_pointer null_;
        node_pointer root_;

    public:
        reference operator*() { return current_->kv; }

        pointer operator->() { return &(current_->kv); }

        map_normal_iterator &operator++() {
            if (current_ == null_)
                current_ = null_;
            else if (current_->right != null_) {
                current_ = current_->right;
                while (current_->left and current_->left != null_)
                    current_ = current_->left;
            } else {
                while (current_->is_right_child())
                    current_ = current_->parent;

                if (current_ != root_)
                    current_ = current_->parent;
                else
                    current_ = null_;
            }
            return *this;
        }

        map_normal_iterator operator++(int) {
            map_normal_iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        map_normal_iterator &operator--() {
            if (current_ == null_)
                current_ = null_;
            else if (current_->left != null_) {
                current_ = current_->left;
                while (current_->right and current_->right != null_)
                    current_ = current_->right;
            } else {
                while (current_->is_left_child())
                    current_ = current_->parent;

                if (current_ != root_)
                    current_ = current_->parent;
                else
                    current_ = null_;
            }
            return *this;
        }

        node_pointer node() const noexcept { return current_; }

        map_normal_iterator operator--(int) {
            map_normal_iterator tmp(*this);
            --(*this);
            return tmp;
        }

        bool operator==(const map_normal_iterator &other) const {
            return current_ == other.current_;
        }

        bool operator!=(const map_normal_iterator &other) const {
            return current_ != other.current_;
        }
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_map_NORMAL_ITERATOR_H
