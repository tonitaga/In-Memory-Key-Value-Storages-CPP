#ifndef TRANSACTIONS_LIBRARY_CPP_UNORDERED_map_SIZE_H
#define TRANSACTIONS_LIBRARY_CPP_UNORDERED_map_SIZE_H

#include <array>
#include <limits>

namespace ttl::detail {
    struct unordered_map_size {
        static const std::array<unsigned long long, 31> sizes;
        static const double kResizeAlpha;

        static std::size_t size(std::size_t index) {
            return sizes[index];
        }
    };

    inline constexpr const std::array<unsigned long long, 31> unordered_map_size::sizes = {
            0ull, 4ull, 12ull, 28ull, 60ull, 124ull,
            252ull, 508ull, 1020ull, 2044ull, 4092ull,
            8188ull, 16380ull, 32764ull, 65532ull, 131068ull,
            262140ull, 524284ull, 1048572ull, 2097148ull, 4194300ull,
            8388604ull, 16777212ull, 33554428ull, 67108860ull, 134217724ull,
            268435452ull, 536870908ull, 1073741820ull, 2147483640ull, std::numeric_limits<unsigned long long>::max()
    };

    inline constexpr double unordered_map_size::kResizeAlpha = 0.75;

    /*
     *
     * 1st array is:  sum of 2nd array from 0 to i element
     * 2nd array is:  count of possible inserted elements before resize
     * 3rd array is: size of hash table
     *
     * [3,  9,  21, 45,  93, 189, 381,  765, 1533, 3069, 6141, 12285, 24573, 49149,  98301, 196605, 393213,  786429, 1572861, 3145725, 6291453, 12582909, 25165821, 50331645, 100663293, 201326589, 402653181,  805306365, 1610612730]
     * [3,  6,  12, 24,  48,  96, 192,  384,  768, 1536, 3072,  6144, 12288, 24576,  49152,  98304, 196608,  393216,  786432, 1572864, 3145728,  6291456, 12582912, 25165824,  50331648, 100663296, 201326592,  402653184,  805306368]
     * [4, 12,  28, 60, 124, 252, 508, 1020, 2044, 4092, 8188, 16380, 32764, 65532, 131068, 262140, 524284, 1048572, 2097148, 4194300, 8388604, 16777212, 33554428, 67108860, 134217724, 268435452, 536870908, 1073741820, 2147483640]
     *
     *
     * [3, 6, 12, 24
    */
}

#endif //TRANSACTIONS_LIBRARY_CPP_UNORDERED_map_SIZE_H
