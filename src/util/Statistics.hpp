#pragma once
#include <algorithm>
#include <numeric>
#include <ranges>
#include <span>
#include <type_traits>

namespace weatherer::util {
class Statistics {
 private:
  static constexpr uint32_t kPlusInf_ = ~((1 << 23) - 1) - ((1 << 23) - 1);

  static float ConvertToFloat(uint32_t n) {
    n += ((1 << 23) - 1);
    if (n & (1 << 31))
      n ^= (1 << 31);
    else
      n = ~n;
    return std::bit_cast<float>(n);
  }

 public:
  Statistics() = delete;
  ~Statistics() = delete;

  template <typename Ty_, size_t Amt_,
            typename = std::enable_if<std::is_arithmetic_v<Ty_>>>
  [[nodiscard]] static constexpr double Mean(std::span<Ty_, Amt_> data) {
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
  }

  template <typename Ty_, size_t Amt_,
            typename = std::enable_if<std::is_arithmetic_v<Ty_>>>
  [[nodiscard]] static constexpr double Median(std::span<Ty_, Amt_> data) {
    if constexpr (std::is_integral_v<Ty_>) {
      auto indices = std::views::iota(0u, ~0u);
      auto counts =
          indices | std::views::transform([&](const uint32_t partition) {
            return std::ranges::count_if(
                data, [&](auto i) { return i <= partition; });
          });
      if (data.size() % 2 == 0) {
        auto lower_bound =
            std::ranges::lower_bound(counts, (1 + data.size()) / 2) -
            counts.begin();
        auto upper_bound =
            std::ranges::upper_bound(counts, (1 + data.size()) / 2) -
            counts.begin();
        return (lower_bound + upper_bound) / 2.0;
      }
      auto b = std::ranges::lower_bound(counts, (1 + data.size()) / 2);
      return b - counts.begin();
    } else if constexpr (std::is_floating_point_v<Ty_>) {
      auto indices = std::views::iota(0u, kPlusInf_ + 1);
      auto counts =
          indices | std::views::transform([&](const uint32_t partition) {
            return std::ranges::count_if(
                data, [&](float i) { return i <= ConvertToFloat(partition); });
          });
      if (data.size() % 2 == 0) {
        auto lower_bound =
            std::ranges::lower_bound(counts, (1 + data.size()) / 2) -
            counts.begin();
        auto upper_bound =
            std::ranges::upper_bound(counts, (1 + data.size()) / 2) -
            counts.begin();
        return ConvertToFloat((lower_bound + upper_bound) / 2.0);
      }
      auto b = std::ranges::lower_bound(counts, (data.size() + 1) / 2);
      return ConvertToFloat(b - counts.begin());
    } else {
      static_assert(std::is_arithmetic_v<Ty_>,
                    "Ty_ must be an arithmetic type");
      return {};
    }
  }
};
}  // namespace weatherer::util
