#pragma once

#include <iostream>
#include <iterator>
#include <algorithm>
#include <array>

namespace weatherer::util {
    class Streams {
    public:
        Streams() = delete;
        ~Streams () = delete;

        template <typename T, size_t N>
        static void os_array(std::ostream& os, const std::array<T, N>& arr, const std::string_view label) {
            os << label << ": ";
            std::copy(arr.begin(), arr.end(), std::ostream_iterator<T>(os, " "));
            os << "\n";
        }
    };
}
