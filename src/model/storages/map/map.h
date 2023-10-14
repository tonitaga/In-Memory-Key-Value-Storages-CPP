#ifndef TRANSACTIONS_LIBRARY_CPP_map_H
#define TRANSACTIONS_LIBRARY_CPP_map_H

#include "map_node.h"
#include "map_normal_iterator.h"

namespace ttl {
    template <typename Key, typename Value, typename Compare = std::less<Key>>
    class map {
    private:
        using node_type      = detail::map_node<Key, Value>;
        using node_pointer   = node_type *;
        using color_type     = detail::map_node_color;

    public:
        using key_type       = typename node_type::key_type;
        using mapped_type    = typename node_type::mapped_type;
        using value_type     = typename node_type::value_type;
        using size_type      = typename node_type::size_type;
        using compare_type   = Compare;

        using iterator       = map_normal_iterator<node_type>;
        using const_iterator = map_normal_iterator<const node_type>;

    private:
        node_pointer null_ = nullptr, root_ = nullptr;

        compare_type compare_;
        size_type size_ {};

    public:
        map() : null_(new node_type), root_(null_), compare_(compare_type{}) {};
        map(const map &other) {
            for (const auto &kv : other)
                insert(kv);
        }

        map &operator=(const map &other) {
            map tmp(other);
            *this = std::move(tmp);
            return *this;
        }

        map(map &&other) noexcept : null_(other.null_), root_(other.root_), compare_(other.compare_), size_(other.size_) {
            other.null_ = nullptr;
            other.root_ = nullptr;
            other.size_ = size_type{};
        }

        map &operator=(map &&other) noexcept {
            null_ = other.null_;
            root_ = other.root_;
            compare_ = other.compare_;
            size_ = other.size_;
            other.null_ = nullptr;
            other.root_ = nullptr;
            other.size_ = size_type{};
            return *this;
        }

        ~map() noexcept {
            if (root_ != null_)
                clear();

            delete null_;
        }

    public:
        [[nodiscard]] size_type size() const noexcept { return size_; }
        [[nodiscard]] bool empty() const noexcept { return size_ == size_type{}; }

    public:
        iterator begin() noexcept {
            node_pointer min = min_node(root_);
            return iterator(min, null_, root_);
        }

        iterator end() noexcept { return iterator(null_, null_, root_); }

    public:
        std::pair<iterator, bool> insert(const value_type &kv) {
            node_pointer node = root_;
            node_pointer parent = nullptr;

            while (!is_null(node)) {
                parent = node;

                if (kv.first == node->kv.first)
                    return std::make_pair(iterator(node, null_, root_), false);

                if (compare_(kv.first, node->kv.first))
                    node = node->left;
                else
                    node = node->right;
            }

            node = new node_type(kv);
            node->parent = parent;
            node->left = null_;
            node->right = null_;
            node->color = color_type::kRed;

            if (parent) {
                if (compare_(node->kv.first, parent->kv.first))
                    parent->left = node;
                else
                    parent->right = node;
            } else {
                root_ = node;
            }

            insersion_fix(node);
            size_++;
            return std::make_pair(iterator(node, null_, root_), true);
        }

        std::pair<iterator, bool> insert(value_type &&kv) {
            node_pointer node = root_;
            node_pointer parent = nullptr;

            while (!is_null(node)) {
                parent = node;

                if (kv.first == node->kv.first)
                    return std::make_pair(iterator(node, null_, root_), false);

                if (compare_(kv.first, node->kv.first))
                    node = node->left;
                else
                    node = node->right;
            }

            node = new node_type(std::move(kv));
            node->parent = parent;
            node->left = null_;
            node->right = null_;
            node->color = color_type::kRed;

            if (parent) {
                if (compare_(node->kv.first, parent->kv.first))
                    parent->left = node;
                else
                    parent->right = node;
            } else {
                root_ = node;
            }

            insersion_fix(node);
            size_++;
            return std::make_pair(iterator(node, null_, root_), true);
        }

        mapped_type &operator[](const key_type &key) {
            auto it = insert(std::make_pair(key, mapped_type{})).first;
            return it->second;
        }

        mapped_type &operator[](key_type &&key) {
            auto it = insert(std::make_pair(std::move(key), mapped_type{})).first;
            return it->second;
        }

        void erase(const key_type &key) {
            node_pointer node = find_pointer(root_, key);
            if (!node or is_null(node))
                return;

            node_pointer child = nullptr, delete_node = nullptr;
            if (is_null(node->left) or is_null(node->right)) {
                delete_node = node;
            } else {
                delete_node = min_node(node->right);
                node->kv = delete_node->kv;
            }

            if (!is_null(delete_node->left)) {
                child = delete_node->left;
            } else {
                child = delete_node->right;
            }

            child->parent = delete_node->parent;
            if (delete_node->parent) {
                if (delete_node->is_left_child()) {
                    delete_node->parent->left = child;
                } else {
                    delete_node->parent->right = child;
                }
            } else {
                root_ = child;
            }

            if (delete_node->color == color_type::kBlack)
                erasion_fix(child);

            delete delete_node;
            size_--;
        }

        void erase(key_type &&key) {
            node_pointer node = find_pointer(root_, std::move(key));
            if (!node or is_null(node))
                return;

            node_pointer child = nullptr, delete_node = nullptr;
            if (is_null(node->left) or is_null(node->right)) {
                delete_node = node;
            } else {
                delete_node = min_node(node->right);
                node->kv = delete_node->kv;
            }

            if (!is_null(delete_node->left)) {
                child = delete_node->left;
            } else {
                child = delete_node->right;
            }

            child->parent = delete_node->parent;
            if (delete_node->parent) {
                if (delete_node->is_left_child()) {
                    delete_node->parent->left = child;
                } else {
                    delete_node->parent->right = child;
                }
            } else {
                root_ = child;
            }

            if (delete_node->color == color_type::kBlack)
                erasion_fix(child);

            delete delete_node;
            size_--;
        }

        void erase(iterator it) {
            erase(it->first);
        }

        iterator find(const key_type &key) {
            node_pointer node = find_pointer(root_, key);
            if (node and !is_null(node))
                return iterator(node, null_, root_);
            return end();
        }

        iterator find(key_type &&key) {
            node_pointer node = find_pointer(root_, std::move(key));
            if (!is_null(node))
                return iterator(node, null_, root_);
            return end();
        }

    private:
        void clear() {
            if (root_ and size_ != size_type{})
                clear_recursive(root_);
        }

        void clear_recursive(node_pointer node) {
            if (node and !is_null(node)) {
                clear_recursive(node->left);
                clear_recursive(node->right);

                delete node;
                size_--;
            }
        }

        bool is_null(node_pointer node) const { return node == null_; }

        void insersion_fix(node_pointer x) {
            while (x != root_ and x->parent->color == color_type::kRed) {
                auto parent = x->parent;
                auto grandparent = parent->parent;
                if (grandparent->left and grandparent->right and
                    grandparent->left->color == color_type::kRed and
                    grandparent->right->color == color_type::kRed) {

                    grandparent->color = color_type::kRed;
                    grandparent->left->color = color_type::kBlack;
                    grandparent->right->color = color_type::kBlack;
                    x = std::move(grandparent);
                } else {
                    if (parent->is_left_child()) {
                        if (x->is_right_child()) {
                            x = x->parent;
                            rotate_left(x);
                        }

                        x->parent->color = color_type::kBlack;
                        x->parent->parent->color = color_type::kRed;
                        rotate_right(grandparent);
                    } else {
                        if (x->is_left_child()) {
                            x = x->parent;
                            rotate_right(x);
                        }

                        x->parent->color = color_type::kBlack;
                        x->parent->parent->color = color_type::kRed;
                        rotate_left(grandparent);
                    }
                }
            }
            root_->color = color_type::kBlack;
        }

        void erasion_fix(node_pointer x) {
            while (x != root_ and x->color == color_type::kBlack) {
                if (x->is_left_child()) {
                    erasion_fix_left(x);
                } else {
                    erasion_fix_right(x);
                }
                null_->parent = nullptr;
            }

            x->color = color_type::kBlack;
        }

        void erasion_fix_left(node_pointer &x) {
            node_pointer uncle = x->parent->right;
            if (uncle->color == color_type::kRed) {
                uncle->color = color_type::kBlack;
                x->parent->color = color_type::kRed;
                rotate_left(x->parent);
                uncle = x->parent->right;
            }

            if (uncle->left->color == color_type::kBlack and uncle->right->color == color_type::kBlack) {
                uncle->color = color_type::kRed;
                x = x->parent;
            } else {
                if (uncle->right->color == color_type::kBlack) {
                    uncle->left->color = color_type::kBlack;
                    uncle->color = color_type::kRed;
                    rotate_right(uncle);
                    uncle = x->parent->right;
                }
                uncle->color = x->parent->color;
                x->parent->color = color_type::kBlack;
                uncle->right->color = color_type::kBlack;
                rotate_left(x->parent);
                x = root_;
            }
        }

        void erasion_fix_right(node_pointer &x) {
            auto uncle = x->parent->left;
            if (uncle->color == color_type::kRed) {
                uncle->color = color_type::kBlack;
                x->parent->color = color_type::kRed;
                rotate_right(x->parent);
                uncle = x->parent->left;
            }
            if (uncle->right->color == color_type::kBlack && uncle->left->color == color_type::kBlack) {
                uncle->color = color_type::kRed;
                x = x->parent;
            } else {
                if (uncle->left->color == color_type::kBlack) {
                    uncle->right->color = color_type::kBlack;
                    uncle->color = color_type::kRed;
                    rotate_left(uncle);
                    uncle = x->parent->left;
                }
                uncle->color = x->parent->color;
                x->parent->color = color_type::kBlack;
                uncle->left->color = color_type::kBlack;
                rotate_right(x->parent);
                x = root_;
            }
        }

        node_pointer find_pointer(node_pointer n, const key_type &key) {
            node_pointer node = n;
            if (!node or is_null(node))
                return null_;

            while (!is_null(node)) {
                if (node->kv.first == key)
                    return node;

                if (compare_(key, node->kv.first))
                    node = node->left;
                else
                    node = node->right;
            }
            return null_;
        }

        node_pointer find_pointer(node_pointer n, key_type &&key) {
            node_pointer node = n;
            if (!node or is_null(node))
                return null_;

            while (!is_null(node)) {
                if (std::move(node->kv.first) == std::move(key))
                    return node;

                if (compare_(std::move(key), std::move(node->kv.first)))
                    node = node->left;
                else
                    node = node->right;
            }
            return null_;
        }


        void rotate_left(node_pointer node) {
            node_pointer y = node->right;
            node_pointer mid = y->left;
            node->right = mid;

            if (y != null_) {
                y->parent = node->parent;
                if (y->left != null_)
                    y->left->parent = node;
            }

            if (node->parent) {
                if (node->is_left_child())
                    node->parent->left = y;
                else
                    node->parent->right = y;
            } else {
                root_ = y;
            }

            y->left = node;
            node->parent = y;
        }

        void rotate_right(node_pointer node) {
            node_pointer x = node->left;
            node_pointer mid = x->right;
            node->left = mid;

            if (x != null_) {
                x->parent = node->parent;
                if (mid != null_)
                    mid->parent = node;
            }

            if (node->parent) {
                if (node->is_left_child())
                    node->parent->left = x;
                else
                    node->parent->right = x;
            } else {
                root_ = x;
            }

            x->right = node;
            node->parent = x;
        }

        node_pointer min_node(node_pointer n) {
            node_pointer node = n;
            if (is_null(root_))
                return node;

            while (node->left and !is_null(node->left))
                node = node->left;

            return node;
        }

        node_pointer max_node(node_pointer n) {
            node_pointer node = n;
            if (is_null(root_))
                return node;

            while (node->right and !is_null(node->right))
                node = node->right;

            return node;
        }
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_map_H
