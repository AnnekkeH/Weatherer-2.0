#pragma once

#include <ostream>
#include <type_traits>

namespace weatherer::util {
template<typename T, typename = std::enable_if<std::is_integral_v<T>>>
class NumericRange {
private:
  T min_;
  T max_;

public:
  explicit NumericRange(T max, T min) : min_(min), max_(max) {
   if (min_ > max_) {
     std::swap(min_, max_);
   }
 }

 NumericRange(const NumericRange& other) : min_(other.min_), max_(other.max_) {}

 NumericRange(NumericRange&& other) noexcept
     : min_(std::move(other.min_)), max_(std::move(other.max_)) {}

 NumericRange& operator=(const NumericRange& other) {
   if (this == &other)
     return *this;
   min_ = other.min_;
   max_ = other.max_;
   return *this;
 }

 NumericRange& operator=(NumericRange&& other) noexcept {
   if (this == &other)
     return *this;
   min_ = std::move(other.min_);
   max_ = std::move(other.max_);
   return *this;
 }

 [[nodiscard]] T GetMin() const { return min_; }
  [[nodiscard]] T GetMax() const { return max_; }
  [[nodiscard]] bool IsInRange(T num) const {
    return num >= min_ && num <= max_;
  }
  [[nodiscard]] constexpr double GetAverage() const {
    return (min_ + max_) / 2.0;
  }

  friend std::ostream& operator<<(std::ostream& os, const NumericRange& obj) {
    os << "Min: " << obj.GetMin() << " Max: " << obj.GetMax();
    return os;
  }
};
}  // namespace weatherer::util