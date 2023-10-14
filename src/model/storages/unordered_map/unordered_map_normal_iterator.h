#ifndef TRANSACTIONS_LIBRARY_CPP_UNORDERED_MAP_NORMAL_ITERATOR_H
#define TRANSACTIONS_LIBRARY_CPP_UNORDERED_MAP_NORMAL_ITERATOR_H

namespace ttl {
    template <typename MainIterator, typename BucketIterator>
    class unordered_map_normal_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = typename MainIterator::value_type;
        using difference_type = typename MainIterator::difference_type;
        using pointer = typename BucketIterator::pointer;
        using reference = typename BucketIterator::reference;

        explicit unordered_map_normal_iterator(MainIterator it) : iterator_main_(it) {
            while (iterator_main_->empty())
                ++iterator_main_;
            iterator_bucket_ = iterator_main_->begin();
        }

        unordered_map_normal_iterator(MainIterator main, BucketIterator bucket) : iterator_main_(main), iterator_bucket_(bucket) {};

        reference operator*() const {
            return *iterator_bucket_;
        }

        pointer operator->() const {
            return iterator_bucket_.operator->();
        }

        unordered_map_normal_iterator &operator++() {
            ++iterator_bucket_;
            if (iterator_bucket_ != iterator_main_->end())
                return *this;

            ++iterator_main_;
            while (iterator_main_->empty())
                ++iterator_main_;

            iterator_bucket_ = iterator_main_->begin();
            return *this;
        }

        unordered_map_normal_iterator operator++(int) {
            unordered_map_normal_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const unordered_map_normal_iterator &other) const {
            return iterator_main_ == other.iterator_main_;
        }

        bool operator!=(const unordered_map_normal_iterator &other) const {
            return iterator_main_ != other.iterator_main_;
        }

    private:
        MainIterator iterator_main_;
        BucketIterator iterator_bucket_;
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_UNORDERED_MAP_NORMAL_ITERATOR_H
