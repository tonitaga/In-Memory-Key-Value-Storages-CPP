#ifndef TRANSACTIONS_LIBRARY_CPP_UNORDERED_MAP_NORMAL_ITERATOR_H
#define TRANSACTIONS_LIBRARY_CPP_UNORDERED_MAP_NORMAL_ITERATOR_H

namespace ttl {
    template <typename MainIterator, typename BucketIterator>
    class UnorderedMapNormalIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = typename MainIterator::value_type;
        using difference_type = typename MainIterator::difference_type;
        using pointer = typename BucketIterator::pointer;
        using reference = typename BucketIterator::reference;

        explicit UnorderedMapNormalIterator(MainIterator it) : iterator_main_(it) {
            while (iterator_main_.operator*().empty())
                ++iterator_main_;
            iterator_bucket_ = iterator_main_.operator*().begin();
        }

        UnorderedMapNormalIterator(MainIterator main, BucketIterator bucket) : iterator_main_(main), iterator_bucket_(bucket) {};

        reference operator*() const {
            return *iterator_bucket_;
        }

        pointer operator->() const {
            return iterator_bucket_.operator->();
        }

        UnorderedMapNormalIterator &operator++() {
            if (iterator_bucket_ != iterator_main_.operator*().end()) {
                ++iterator_bucket_;
                if (iterator_bucket_ == iterator_main_.operator*().end()) {
                    ++iterator_main_;
                    while (iterator_main_.operator*().empty())
                        ++iterator_main_;

                    iterator_bucket_ = iterator_main_.operator*().begin();
                }
                return *this;
            }

            ++iterator_main_;
            while (iterator_main_.operator*().empty())
                ++iterator_main_;

            iterator_bucket_ = iterator_main_.operator*().begin();
            return *this;
        }

        UnorderedMapNormalIterator operator++(int) {
            UnorderedMapNormalIterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const UnorderedMapNormalIterator &other) const {
            return iterator_main_ == other.iterator_main_;
        }

        bool operator!=(const UnorderedMapNormalIterator &other) const {
            return iterator_main_ != other.iterator_main_;
        }

    private:
        MainIterator iterator_main_;
        BucketIterator iterator_bucket_;
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_UNORDERED_MAP_NORMAL_ITERATOR_H
