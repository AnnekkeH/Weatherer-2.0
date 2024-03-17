#pragma once

#include "api/PvDataProcessor.hpp"
#include "api/models/Coordinates.hpp"
#include "util/Date.hpp"

namespace weatherer {
/**
 * @brief PvHandler class manages and processes photovoltaic (PV) system data.
 *
 * The PvHandler class is responsible for aggregating, managing, and providing access to
 * weather data related to photovoltaic systems. It encapsulates functionality for
 * fetching weather data, validating solar panel parameters, and outputting relevant
 * information. The class is designed to abstract the underlying complexities of data
 * processing from the user, offering a streamlined interface for working with
 * photovoltaic system data.
 */
class PvHandler {
 private:
  Coordinates coords_;
  util::TimeFrame time_frame_;
  PvCollectionPtr pv_collection_;

/**
 * @brief Validates the efficiency domain of a solar panel (0 <= efficiency <= 1).
 * @param panel_eff The efficiency of the solar panel.
 * @return True if the efficiency is within the valid domain, false otherwise.
 */
  [[nodiscard]] inline static bool VaildateSolarPanelEfficiency(const double panel_eff);

/**
 * @brief Validates the area domain of a solar panel (area > 0).
 * @param panel_area The area of the solar panel.
 * @return True if the area is within the valid domain, false otherwise.
 */
  [[nodiscard]] inline static bool ValidateSolarPanelArea(const double panel_area);

 public:
  [[nodiscard]] explicit PvHandler(Coordinates const& coords,
                          util::TimeFrame const& time_frame);
  ~PvHandler();
  PvHandler(const PvHandler& other);
  PvHandler(PvHandler&& other) noexcept;
  PvHandler& operator=(const PvHandler& other);
  PvHandler& operator=(PvHandler&& other) noexcept;

  // void PrintData() const;

/**
 * @brief Appends aggregated weather data to the specified output stream.
 * @param os The output stream to append data to.
 *
 * Appends the aggregated weather data for each day to the provided
 * output stream. It iterates over the stored PvData instances in the PvCollection,
 * adding the date and detailed weather information for each day to the output stream.
 */
  void OutputData(std::ostream& os) const;
  // void PrintDailyEnergyYeild(const double panel_eff,
                                 // const double panel_area) const;

/**
 * @brief Appends daily energy yield information to the specified output stream.
 * @param os The output stream to append data to.
 * @param panel_eff The efficiency of the solar panel.
 * @param panel_area The area of the solar panel.
 * @throws std::invalid_argument if the solar panel efficiency or area is outside the valid domain.
 *
 * Calculates and appends the daily energy yield information for each
 * day, considering the specified solar panel efficiency and area. It validates the
 * input parameters before processing and appends the date along with the calculated
 * daily energy yield in kilowatt-hours to the output stream.
 */
  void OutputDailyEnergyYeild(std::ostream& os,
                                 const double panel_eff,
                                 const double panel_area) const;

  [[nodiscard]] inline PvCollectionPtr GetPvCollection() const;

};
}  // namespace weatherer
