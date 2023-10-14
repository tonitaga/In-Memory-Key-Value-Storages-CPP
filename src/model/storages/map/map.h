#ifndef TRANSACTIONS_LIBRARY_CPP_MAP_H
#define TRANSACTIONS_LIBRARY_CPP_MAP_H

#include "map_node.h"
#include "map_normal_iterator.h"

#include <vector>

namespace ttl {
    template <typename Key, typename Value, typename Compare = std::less<Key>>
    class Map {
    public:
        using node_type      = MapNode<Key, Value>;
        using node_pointer   = node_type *;
        using key_type       = typename node_type::key_type;
        using mapped_type    = typename node_type::mapped_type;
        using value_type     = typename node_type::value_type;
        using size_type      = typename node_type::size_type;
        using compare_type   = Compare;

        using iterator       = MapNormalIterator<node_type>;
        using const_iterator = MapNormalIterator<const node_type>;

    private:
        compare_type cmp_;

        node_pointer root_ = nullptr, null_ = nullptr;
        size_type size_ = 0;
    public:
        Map() : root_(new node_type), null_(new node_type) {
            root_->parent = null_;
            root_->left = null_;
            root_->right = null_;
            root_->is_red = false;
            root_->count = 0;
            null_->parent = root_;
            null_->left = null_;
            null_->right = null_;
            null_->is_red = false;
            null_->count = 0;
        }

        ~Map() {
            node_pointer node = root_;
            while (node->left != null_)
                node = node->left;

            while (!empty()) {
                if (node->parent != null_) {
                    auto parent = node->parent;
                    if (parent->left != null_)
                        delete parent->left;
                }
            }

            delete root_;
            delete null_;
        }

        mapped_type &operator[](const key_type &key) {
            iterator it = find(key);
            if (it == end()) {
                value_type kv(key, mapped_type());
                it = insert(std::move(kv)).first;
            }
            return it->second;
        }

        mapped_type &operator[](key_type &&key) {
            iterator it = find(key);
            if (it == end()) {
                value_type kv(std::move(key), mapped_type());
                it = insert(std::move(kv)).first;
            }
            return it->second;
        }

        std::pair<iterator, bool> insert(const value_type &kv) {
            return insert_internal(kv);
        }

        std::pair<iterator, bool> insert(value_type &&kv) {
            return insert_internal(std::move(kv));
        }

        void erase(iterator pos) {
            if (pos == end())
                return;

            node_pointer node_to_erase = pos.node();
            node_pointer successor = nullptr;

            if (node_to_erase->left == null_ || node_to_erase->right == null_)
                successor = node_to_erase;
            else
                successor = next_node(node_to_erase);

            node_pointer child = nullptr;
            if (successor->left != null_)
                child = successor->left;
            else
                child = successor->right;

            child->parent = successor->parent;
            if (successor->parent == null_)
                root_ = child;
            else if (successor == successor->parent->left)
                successor->parent->left = child;
            else
                successor->parent->right = child;

            if (successor != node_to_erase)
                node_to_erase->key_value = std::move(successor->key_value);

            if (!successor->is_red)
                erase_fixup(child, successor->parent);

            delete successor;
            --size_;
        }

        void erase(const key_type &key) {
            iterator it = find(key);
            if (it != end())
                erase(it);
        }

        void erase(key_type &&key) {
            iterator it = find(std::move(key));
            if (it != end())
                erase(it);
        }

        [[nodiscard]] bool empty() const noexcept {
            return size_ == 0;
        }

        [[nodiscard]] size_type size() const noexcept {
            return size_;
        }

        iterator find(const key_type &key) {
            node_pointer node = root_;
            while (node != null_) {
                if (cmp_(key, node->key_value.first))
                    node = node->left;
                else if (cmp_(node->key_value.first, key))
                    node = node->right;
                else
                    return iterator(node);
            }
            return end();
        }

        iterator find(key_type &&key) {
            node_pointer node = root_;
            while (node != null_) {
                if (cmp_(key, node->key_value.first))
                    node = node->left;
                else if (cmp_(node->key_value.first, key))
                    node = node->right;
                else
                    return iterator(node);
            }
            return end();
        }

        iterator begin() noexcept {
            node_pointer node = root_;
            while (node->left != null_)
                node = node->left;
            return iterator(node);
        }

        const_iterator begin() const noexcept {
            node_pointer node = root_;
            while (node->left != null_)
                node = node->left;
            return const_iterator(node);
        }

        iterator end() noexcept {
            return iterator(null_);
        }

        const_iterator end() const noexcept {
            return const_iterator(null_);
        }

    private:
        template <typename KV>
        std::pair<iterator, bool> insert_internal(KV &&kv) {
            node_pointer parent = null_;
            node_pointer node = root_;

            while (node != null_) {
                parent = node;
                if (cmp_(kv.first, node->key_value.first))
                    node = node->left;
                else if (cmp_(node->key_value.first, kv.first))
                    node = node->right;
                else
                    return std::make_pair(iterator(node), false);
            }

            node_pointer new_node = new node_type(std::forward<KV>(kv));
            new_node->parent = parent;
            new_node->left = null_;
            new_node->right = null_;
            new_node->is_red = true;

            if (parent == null_)
                root_ = new_node;
            else if (cmp_(new_node->key_value.first, parent->key_value.first))
                parent->left = new_node;
            else
                parent->right = new_node;

            insert_fixup(new_node);
            ++size_;

            return std::make_pair(iterator(new_node), true);
        }

        void insert_fixup(node_pointer node) {
            while (node->parent->is_red) {
                if (node->parent == node->parent->parent->left) {
                    node_pointer uncle = node->parent->parent->right;
                    if (uncle->is_red) {
                        node->parent->is_red = false;
                        uncle->is_red = false;
                        node->parent->parent->is_red = true;
                        node = node->parent->parent;
                    } else {
                        if (node == node->parent->right) {
                            node = node->parent;
                            rotate_left(node);
                        }
                        node->parent->is_red = false;
                        node->parent->parent->is_red = true;
                        rotate_right(node->parent->parent);
                    }
                } else {
                    node_pointer uncle = node->parent->parent->left;
                    if (uncle->is_red) {
                        node->parent->is_red = false;
                        uncle->is_red = false;
                        node->parent->parent->is_red = true;
                        node = node->parent->parent;
                    } else {
                        if (node == node->parent->left) {
                            node = node->parent;
                            rotate_right(node);
                        }
                        node->parent->is_red = false;
                        node->parent->parent->is_red = true;
                        rotate_left(node->parent->parent);
                    }
                }
            }
            root_->is_red = false;
        }

        void erase_fixup(node_pointer node, node_pointer parent) {
            while (node != root_ && !node->is_red) {
                if (node == parent->left) {
                    node_pointer sibling = parent->right;
                    if (sibling->is_red) {
                        sibling->is_red = false;
                        parent->is_red = true;
                        rotate_left(parent);
                        sibling = parent->right;
                    }
                    if (!sibling->left->is_red && !sibling->right->is_red) {
                        sibling->is_red = true;
                        node = parent;
                        parent = node->parent;
                    } else {
                        if (!sibling->right->is_red) {
                            sibling->left->is_red = false;
                            sibling->is_red = true;
                            rotate_right(sibling);
                            sibling = parent->right;
                        }
                        sibling->is_red = parent->is_red;
                        parent->is_red = false;
                        sibling->right->is_red = false;
                        rotate_left(parent);
                        node = root_;
                    }
                } else {
                    node_pointer sibling = parent->left;
                    if (sibling->is_red) {
                        sibling->is_red = false;
                        parent->is_red = true;
                        rotate_right(parent);
                        sibling = parent->left;
                    }
                    if (!sibling->left->is_red && !sibling->right->is_red) {
                        sibling->is_red = true;
                        node = parent;
                        parent = node->parent;
                    } else {
                        if (!sibling->left->is_red) {
                            sibling->right->is_red = false;
                            sibling->is_red = true;
                            rotate_left(sibling);
                            sibling = parent->left;
                        }
                        sibling->is_red = parent->is_red;
                        parent->is_red = false;
                        sibling->left->is_red = false;
                        rotate_right(parent);
                        node = root_;
                    }
                }
            }
            node->is_red = false;
        }

        void rotate_left(node_pointer node) {
            node_pointer right_child = node->right;
            node->right = right_child->left;
            if (right_child->left != null_)
                right_child->left->parent = node;
            right_child->parent = node->parent;
            if (node->parent == null_)
                root_ = right_child;
            else if (node == node->parent->left)
                node->parent->left = right_child;
            else
                node->parent->right = right_child;
            right_child->left = node;
            node->parent = right_child;
        }

        void rotate_right(node_pointer node) {
            node_pointer left_child = node->left;
            node->left = left_child->right;
            if (left_child->right != null_)
                left_child->right->parent = node;
            left_child->parent = node->parent;
            if (node->parent == null_)
                root_ = left_child;
            else if (node == node->parent->left)
                node->parent->left = left_child;
            else
                node->parent->right = left_child;
            left_child->right = node;
            node->parent = left_child;
        }

        node_pointer next_node(node_pointer node) {
            if (node->right != null_) {
                node = node->right;
                while (node->left != null_)
                    node = node->left;
                return node;
            } else {
                node_pointer parent = node->parent;
                while (parent != null_ && node == parent->right) {
                    node = parent;
                    parent = parent->parent;
                }
                return parent;
            }
        }

        void erase_subtree(node_pointer node) {
            if (node != null_) {
                erase_subtree(node->left);
                erase_subtree(node->right);
                delete node;
            }
        }
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_MAP_H
