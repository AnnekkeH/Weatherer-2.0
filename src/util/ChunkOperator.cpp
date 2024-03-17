#include <vector>
#include <string>

#include "util/ChunkOperator.hpp"

std::vector<std::string> weatherer::util::SplitString(std::string const& str, const std::string_view delimiter) {
    std::vector<std::string> result;
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::vector<std::string> res;

    while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
      std::string token = str.substr (pos_start, pos_end - pos_start);
      pos_start = pos_end + delim_len;
      res.push_back (token);
    }

    res.push_back(str.substr (pos_start));
    return res;
}
