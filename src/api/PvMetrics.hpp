#pragma once
#include <cmath>

#include "models/Coordinates.hpp"
#include "models/PvData.hpp"

namespace weatherer {
class PvMetrics {
 private:
  static double CalculateSolarIrradiance(const PvData& pv_data);
  static int CalculateSolarNoonTime(const PvData& pv_data);

 public:
  PvMetrics() = delete;
  ~PvMetrics() = delete;
  static double CalculateDailyEnergyYeild(PvData const& pv_data,
                                          Coordinates const& coordinates,
                                          std::string const& date,
                                          const double panel_eff,
                                          const double panel_area);
};
}  // namespace weatherer
