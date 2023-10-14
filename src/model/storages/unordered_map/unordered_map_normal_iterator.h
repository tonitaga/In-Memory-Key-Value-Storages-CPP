#ifndef TRANSACTIONS_LIBRARY_CPP_UNORDERED_map_NORMAL_ITERATOR_H
#define TRANSACTIONS_LIBRARY_CPP_UNORDERED_map_NORMAL_ITERATOR_H

#include <iterator>

namespace ttl {
    template <typename TableIterator, typename BucketIterator>
    class unordered_map_normal_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using pointer = typename BucketIterator::pointer;
        using reference = typename BucketIterator::reference;

        explicit unordered_map_normal_iterator(TableIterator it) : table_(it) {
            while (table_->empty())
                ++table_;
            bucket_ = table_->begin();
        }

        unordered_map_normal_iterator(TableIterator main, BucketIterator bucket) : table_(main), bucket_(bucket) {};

        reference operator*() const {
            return *bucket_;
        }

        pointer operator->() const {
            return bucket_.operator->();
        }

        unordered_map_normal_iterator &operator++() {
            ++bucket_;
            if (bucket_ != table_->end())
                return *this;

            ++table_;
            while (table_->empty())
                ++table_;

            bucket_ = table_->begin();
            return *this;
        }

        unordered_map_normal_iterator operator++(int) {
            unordered_map_normal_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const unordered_map_normal_iterator &other) const {
            return table_ == other.table_;
        }

        bool operator!=(const unordered_map_normal_iterator &other) const {
            return table_ != other.table_;
        }

    private:
        TableIterator table_;
        BucketIterator bucket_;
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_UNORDERED_map_NORMAL_ITERATOR_H
