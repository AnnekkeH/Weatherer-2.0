#pragma once

namespace weatherer {
/**
 * @brief Represents geographical coordinates (latitude and longitude).
 *
 * The Coordinates class encapsulates latitude and longitude values, providing
 * a convenient representation for geographical locations. It includes methods
 * to access the latitude and longitude values.
 */
class Coordinates {
 private:
  double latitude_;
  double longitude_;

 public:
  [[nodiscard]] explicit Coordinates(const double latitude, const double longitude);
  ~Coordinates();
  Coordinates(const Coordinates& other);
  Coordinates(Coordinates&& other) noexcept;
  Coordinates& operator=(const Coordinates& other);
  Coordinates& operator=(Coordinates&& other) noexcept;
  [[nodiscard]] double GetLatitude() const;
  [[nodiscard]] double GetLongitude() const;
};
}  // namespace weatherer