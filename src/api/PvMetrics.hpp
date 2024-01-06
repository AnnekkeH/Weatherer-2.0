#pragma once

#include "models/Coordinates.hpp"
#include "models/PvData.hpp"

namespace weatherer {
class PvMetrics {
private:
/**
 * @brief Calculates the solar irradiance based on direct and diffuse radiation data.
 * @param pv_data The photovoltaic data containing radiation information.
 * @return The calculated solar irradiance.
 *
 * This method computes the solar irradiance by summing the direct and diffuse radiation
 * for each hour and then calculating the mean of the hourly sum. The result represents
 * the solar irradiance, which is a crucial factor in determining the energy yield of
 * photovoltaic systems.
 */
  static double CalculateSolarIrradiance(const PvData& pv_data);

  /**
   * @brief Calculates the solar noon time based on sunrise and sunset information.
   * @param pv_data The photovoltaic data containing sunrise and sunset times.
   * @return The calculated solar noon hour.
   *
   * This method determines the solar noon time by finding the midpoint between the
   * sunrise and sunset times. It calculates the duration between them, computes the
   * midpoint duration, and adds it to the sunrise time. The result represents the hour
   * of the day when solar noon occurs.
   */
  static int CalculateSolarNoonTime(const PvData& pv_data);

public:
  // Prevent instantiation of the PvMetrics class.
  PvMetrics() = delete;
  ~PvMetrics() = delete;

/**
 * @brief Calculates the daily energy yield of a photovoltaic system.
 * @param pv_data The photovoltaic data containing weather information.
 * @param coordinates The geographical coordinates of the location.
 * @param date The date for which the energy yield is calculated.
 * @param panel_eff The efficiency of the solar panel (between 0 and 1).
 * @param panel_area The area of the solar panel (in square meters).
 * @return The calculated daily energy yield in kilowatt-hours.
 *
 * This method computes the daily energy yield of a photovoltaic system based on
 * weather data, location coordinates, and solar panel parameters. It considers
 * factors such as temperature, cloud cover, solar irradiance, and incident angle
 * to adjust the irradiance. The final energy yield is obtained by multiplying the
 * adjusted irradiance with the solar panel efficiency and area.
 */
  static double CalculateDailyEnergyYeild(PvData const& pv_data,
                                          Coordinates const& coordinates,
                                          std::string const& date,
                                          const double panel_eff,
                                          const double panel_area);
};
}  // namespace weatherer
