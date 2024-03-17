#pragma once
#include <array>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>

namespace weatherer::util {
/**
 * @brief ChunkVector slices a vector into fixed-size chunks and returns them as arrays.
 * @tparam Ty_ The type of elements in the input vector.
 * @tparam Amt_ The size of each chunk array.
 * @param data The input vector to be chunked.
 * @param number The size of each chunk.
 * @return A vector of arrays containing the sliced chunks or an empty vector.
 * @throws std::out_of_range if the input data size is not a multiple of the specified number.
 *
 * This utility function takes a vector and divides it into fixed-size chunks
 * specified by the 'number' parameter. The resulting chunks are represented
 * as arrays and stored in a vector. If the input size is not a multiple of
 * 'number', an exception is thrown. If the input vector is empty, an empty vector
 * is returned.
 */
template <typename Ty_, std::size_t Amt_>
std::vector<std::array<Ty_, Amt_>> ChunkVector(
    const std::vector<Ty_>& data, std::size_t number) {
  [[unlikely]] if (data.size() % number != 0) {
    throw std::out_of_range("Input data size is not a multiple of number");
  }

  [[unlikely]] if (data.empty()) { return {}; }

  std::vector<std::array<Ty_, Amt_>> result{};
  result.reserve(data.size() / number);
  for (auto const& chunk : data | std::ranges::views::chunk(Amt_)) {
    std::array<Ty_, Amt_> chunked{};
    std::copy(chunk.begin(), chunk.end(), chunked.begin());
    result.push_back(chunked);
  }

  return result;
}

std::vector<std::string> SplitString(std::string const& str, std::string_view delimiter);
} // namespace weatherer::util
