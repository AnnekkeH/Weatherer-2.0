#pragma once

namespace weatherer {
struct Coordinates {
 private:
  double latitude_;
  double longitude_;

 public:
  [[nodiscard]] Coordinates(const double latitude, const double longitude);
  ~Coordinates();
  Coordinates(const Coordinates& other);
  Coordinates(Coordinates&& other) noexcept;
  Coordinates& operator=(const Coordinates& other);
  Coordinates& operator=(Coordinates&& other) noexcept;
  [[nodiscard]] double GetLatitude() const;
  [[nodiscard]] double GetLongitude() const;
};
}  // namespace weatherer