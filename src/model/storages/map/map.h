#ifndef TRANSACTIONS_LIBRARY_CPP_MAP_H
#define TRANSACTIONS_LIBRARY_CPP_MAP_H

#include "map_node.h"
#include "map_normal_iterator.h"

namespace ttl {
    template <typename Key, typename Value, typename Compare = std::less<Key>>
    class Map {
    public:
        using key_type       = Key;
        using mapped_type    = Value;
        using value_type     = std::pair<Key, Value>;
        using compare_type   = Compare;
        using node_type      = MapNode<Key, Value>;
        using node_pointer   = node_type *;
        using size_type      = std::size_t;
        using iterator       = MapNormalIterator<node_type>;
        using const_iterator = MapNormalIterator<const node_type>;

    private:
        compare_type cmp_;

        node_pointer root_ = nullptr, nil_ = nullptr;
        size_type size_;

    public:
        Map() {
            initialize();
        }

        ~Map() noexcept {
//            clear(root_);
//            delete nil_;
        }

        [[nodiscard]] bool empty() const noexcept { return size_ == size_type{}; }
        [[nodiscard]] size_type size() const noexcept { return size_; }

        iterator insert(const std::pair<key_type, mapped_type> &kv) { return iterator(insert(kv, compare_type{})); }
        iterator insert(std::pair<key_type, mapped_type> &&kv) { return iterator(insert(std::move(kv), compare_type{})); }

        mapped_type& operator[](const key_type& key) {
            iterator it = find(key);
            if (it != end()) {
                return it->second;
            } else {
                auto newNode = insert(std::make_pair(key, mapped_type{}));
                return newNode->second;
            }
        }

        mapped_type& operator[](key_type&& key) {
            iterator it = find(std::move(key));
            if (it != end()) {
                return it->second;
            } else {
                auto newNode = insert(std::make_pair(std::move(key), mapped_type{}));
                return newNode->second;
            }
        }

        iterator find(const key_type &key) {
            iterator it(find(key, compare_type{}));
            if (it->first != key)
                return end();
            return it;
        }

        iterator find(key_type &&key) {
            iterator it(find(std::move(key), compare_type{}));
            if (it->first != std::move(key))
                return end();
            return it;
        }

        void erase(iterator it) { erase(it.node()); }

        void erase(const key_type &key) {
            node_pointer p = find(key);
            if (p->key_value.first != key)
                return;

            erase(p);
        }

        void erase(key_type &&key) {
            node_pointer p = find(std::move(key));
            if (p->key_value.first != std::move(key))
                return;

            erase(p);
        }

        iterator begin() noexcept {
            if (empty())
                return iterator(nil_);
            return iterator(min_element(root_));
        }

        const_iterator begin() const noexcept {
            if (empty())
                return const_iterator(nil_);
            return const_iterator(min_element(root_));
        }

        iterator end() noexcept { return iterator(nil_); }
        const_iterator end() const noexcept { return const_iterator(nil_); }

    private:
        void initialize() {
            root_ = new node_type;
            nil_  = new node_type;

            root_->parent = nil_;
            root_->left = nil_;
            root_->right = nil_;
            root_->is_red = false;
            root_->count = 0;

            nil_->parent = root_;
            nil_->left = nil_;
            nil_->right = nil_;
            nil_->is_red = false;
            nil_->count = 0;
            size_ = 0;
        }

        void clear(node_pointer node) {
            if (node != nil_) {
                clear(node->left);
                clear(node->right);
                delete node;
            }
        }

        node_pointer insert(const std::pair<key_type, mapped_type> &kv, compare_type compare) {
            size_++;
            if (size_ == 1) {
                key_type *ptr = (key_type *)(&root_->key_value.first);
                (*ptr) = kv.first;
                root_->key_value.second = kv.second;
                root_->count++;
                nil_->parent = root_;
                return root_;
            }

            node_pointer p = root_, q = p;
            while (p != nil_) {
                q = p;
                if (p->key_value.first == kv.first) return p;
                if (compare(kv.first, p->key_value.first))
                    p = p->left;
                else
                    p = p->right;
            }

            p = q;
            node_pointer n = new node_type(kv);
            n->parent = p;

            if (compare(kv.first, p->key_value.first))
                p->left = n;
            else
                p->right = n;

            n->is_red = true;
            n->left = nil_;
            n->right = nil_;
            n->count = 0;

            for (node_pointer i = n; i != nil_; i = i->parent)
                i->count++;

            if (p->is_red)
                insertion_fix(n);

            nil_->parent = root_;
            return n;
        }

        node_pointer insert(std::pair<key_type, mapped_type> &&kv, compare_type compare) {
            size_++;
            if (size_ == 1) {
                key_type *ptr = (key_type *)(&root_->key_value.first);
                (*ptr) = std::move(kv.first);
                root_->key_value.second = std::move(kv.second);
                root_->count++;
                nil_->parent = root_;
                return root_;
            }

            node_pointer p = root_, q = p;
            while (p != nil_) {
                q = p;
                if (p->key_value.first == kv.first) return p;
                if (compare(kv.first, p->key_value.first))
                    p = p->left;
                else
                    p = p->right;
            }

            p = q;
            node_pointer n = new node_type(std::move(kv));
            n->parent = p;

            if (compare(n->key_value.first, p->key_value.first))
                p->left = n;
            else
                p->right = n;

            n->is_red = true;
            n->left = nil_;
            n->right = nil_;
            n->count = 0;

            for (node_pointer i = n; i != nil_; i = i->parent)
                i->count++;

            if (p->is_red)
                insertion_fix(n);

            nil_->parent = root_;
            return n;
        }

        void erase(node_pointer x) {
            if (x == nil_)
                return;

            if (size_ != 1) {
                node_pointer a = min_element(x->right);
                if (a == nil_)
                    a = x;
                else {
                    key_type *ptr = (key_type *)(&x->key_value.first);
                    (*ptr) = std::move(a->key_value.first);
                    x->key_value.second = std::move(a->key_value.second);
                }


                if (a->left != nil_ or a->right != nil_) {
                    node_pointer b;
                    if (a->right != nil_)
                        b = a->right;
                    else
                        b = a->left;

                    if (a == root_) {
                        root_ = b;
                        b->is_red = false;
                        b->parent = nil_;
                    } else {
                        if (a->parent->left == a)
                            a->parent->left = b;
                        else
                            a->parent->right = b;
                        b->parent = a->parent;

                        for (node_pointer i = b->parent; i != nil_; i = i->parent)
                            i->count--;

                        if (a->is_red or b->is_red)
                            b->is_red = false;
                        else
                            erasion_fix(b);
                    }
                } else {
                    if (!a->is_red)
                        erasion_fix(a);
                    if (a->parent->left == a)
                        a->parent->left = nil_;
                    else
                        a->parent->right = nil_;

                    for (node_pointer i = a->parent; i != nil_; i = i->parent)
                        i->count--;
                }

                delete a;
            }
            size_--;
            key_type *ptr = (key_type *)(&root_->key_value.first);
            (*ptr) = "";
            nil_->parent = root_;
        }

        node_pointer find(const key_type &key, compare_type compare) {
            node_pointer p = root_,  q = p;

            while (p != nil_) {
                q = p;
                if (key == p->key_value.first)
                    return p;
                if (compare(key, p->key_value.first))
                    p = p->left;
                else
                    p = p->right;
            }

            return q;
        }

        node_pointer find(key_type &&key, compare_type compare) {
            node_pointer p = root_;

            while (p != nil_) {
                if (std::move(key) == p->key_value.first)
                    return p;
                if (compare(std::move(key), p->key_value.first))
                    p = p->left;
                else
                    p = p->right;
            }

            return p;
        }

        node_pointer sibling_node(node_pointer x) {
            if (x == root_)
                return nil_;
            if (x == x->parent->left)
                return x->parent->right;
            return x->parent->left;
        }

        void insertion_fix(node_pointer p) {
            if (p->parent == root_) {
                root_->is_red = false;
                return;
            }

            node_pointer s = sibling_node(p->parent);
            node_pointer a = p->parent->parent;

            if (s->is_red) {
                s->is_red = false;
                p->parent->is_red = false;

                if (a != root_) {
                    a->is_red = true;
                    if (a->parent->is_red)
                        insertion_fix(a);
                }
            } else {
                if (p->parent == a->left) {
                    if (p == p->parent->right) {
                        left_rotate(p);
                        right_rotate(p);
                        p->is_red = false;
                    } else {
                        right_rotate(p->parent);
                        p->parent->is_red = false;
                    }
                    a->is_red = true;
                } else {
                    if (p == p->parent->right) {
                        right_rotate(p);
                        left_rotate(p);
                        p->is_red = false;
                    } else {
                        left_rotate(p->parent);
                        p->parent->is_red = false;
                    }
                    a->is_red = true;
                }
            }
        }

        void erasion_fix(node_pointer x) {
            if (x == root_) {
                x->is_red = false;
                return;
            }

            node_pointer p = x->parent;
            if (x == p->left) {
                node_pointer s = p->right, l = s->left, r = s->right;
                if (r->is_red) {
                    left_rotate(s);
                    std::swap(p->is_red, s->is_red);
                    r->is_red = false;
                } else if (l->is_red) {
                    right_rotate(l);
                    left_rotate(l);
                    l->is_red = p->is_red;
                    p->is_red = false;
                } else if (s->is_red) {
                    left_rotate(s);
                    p->is_red = true;
                    s->is_red = false;
                    erasion_fix(x);
                } else if (p->is_red) {
                    p->is_red = false;
                    s->is_red = true;
                } else {
                    s->is_red = true;
                    erasion_fix(p);
                }
            } else {
                node_pointer s = p->left, l = s->left, r = s->right;
                if (l->is_red) {
                    right_rotate(s);
                    std::swap(p->is_red, s->is_red);
                    l->is_red = false;
                } else if (r->is_red) {
                    left_rotate(r);
                    right_rotate(r);
                    r->is_red = p->is_red;
                    p->is_red = false;
                } else if (s->is_red) {
                    right_rotate(s);
                    p->is_red = true;
                    s->is_red = false;
                    erasion_fix(x);
                } else if (p->is_red) {
                    p->is_red = false;
                    s->is_red = true;
                } else {
                    s->is_red = true;
                    erasion_fix(p);
                }
            }
        }

        void left_rotate(node_pointer b) {
            node_pointer a = b->parent, p = b->left, g = a->parent;
            if (a == root_) {
                root_ = b;
            } else {
                if (a == g->left)
                    g->left = b;
                else
                    g->right = b;
            }

            if (b == root_)
                b->parent = nil_;
            else
                b->parent = g;

            b->left = a;
            a->parent = b;
            a->right = p;

            if (p != nil_)
                p->parent = a;
            b->count = a->count;
            a->count = a->left->count + a->right->count + 1;
        }

        void right_rotate(node_pointer b) {
            node_pointer a = b->parent, p = b->right, g = a->parent;
            if (a == root_) {
                root_ = b;
            } else {
                if (a == g->left)
                    g->left = b;
                else
                    g->right = b;
            }

            if (b == root_)
                b->parent = nil_;
            else
                b->parent = g;

            b->right = a;
            a->parent = b;
            a->left = p;

            if (p != nil_)
                p->parent = a;
            a->count = b->count;
            b->count = b->left->count + b->right->count + 1;
        }

        node_pointer min_element(node_pointer p) {
            while (p->left != nil_)
                p = p->left;
            return p;
        }
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_MAP_H
