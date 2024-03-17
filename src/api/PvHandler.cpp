#include "PvHandler.hpp"

#include <ostream>
// #include <iomanip>
// #include <print>

#include "PvMetrics.hpp"


/**
 * @param coords The geographical coordinates.
 * @param time_frame The time frame for data aggregation.
*/
weatherer::PvHandler::PvHandler(Coordinates const& coords,
                                util::TimeFrame const& time_frame)
    : coords_{coords},
      time_frame_{time_frame},
      pv_collection_(PvDataProcessor::CollectData(coords, time_frame)) {}

weatherer::PvHandler::~PvHandler() = default;

weatherer::PvHandler::PvHandler(const PvHandler& other) = default;

weatherer::PvHandler::PvHandler(PvHandler&& other) noexcept
    : coords_(std::move(other.coords_)),
      time_frame_(std::move(other.time_frame_)),
      pv_collection_(std::move(other.pv_collection_)) {}

weatherer::PvHandler& weatherer::PvHandler::operator=(const PvHandler& other) {
  if (this == &other)
    return *this;
  coords_ = other.coords_;
  time_frame_ = other.time_frame_;
  pv_collection_ = other.pv_collection_;
  return *this;
}

weatherer::PvHandler& weatherer::PvHandler::operator=(
    PvHandler&& other) noexcept {
  if (this == &other)
    return *this;
  coords_ = other.coords_;
  time_frame_ = other.time_frame_;
  pv_collection_ = std::move(other.pv_collection_);
  return *this;
}

bool weatherer::PvHandler::VaildateSolarPanelEfficiency(
    const double panel_eff) {
  [[unlikely]] if (panel_eff < 0 || panel_eff > 1) { return false; }
  return true;
}

bool weatherer::PvHandler::ValidateSolarPanelArea(const double panel_area) {
  [[unlikely]] if (panel_area <= 0) { return false; }
  return true;
}

// void weatherer::PvHandler::PrintData() const {
//   for (const auto& [key, value] : *pv_collection_) {
//     std::println("{}", key);
//     std::cout << *value << "\n";
//   }
// }

void weatherer::PvHandler::OutputData(std::ostream& os) const {
  for (const auto& [key, value] : *pv_collection_) {
    os << key << "\n" << *value << "\n";
  }
}

// void weatherer::PvHandler::PrintDailyEnergyYeild(
//     const double panel_eff, const double panel_area) const {
//   if (!VaildateSolarPanelEfficiency(panel_eff)) {
//     throw std::invalid_argument(
//         "Solar panel efficiency must be a value between 0 and 1 (inclusive)");
//   }
//
//   if (!ValidateSolarPanelArea(panel_area)) {
//     throw std::invalid_argument(
//         "Solar panel area must be a value greater than 0");
//   }
//
//   for (const auto& [key, value] : *pv_collection_) {
//     std::println("{}", key);
//     std::println("{} kWh\n",
//                  std::round(PvMetrics::CalculateDailyEnergyYeild(
//                                 *value, coords_, key, panel_eff, panel_area) *
//                             1000.0) /
//                      1000.0);
//   }
// }

void weatherer::PvHandler::OutputDailyEnergyYeild(
    std::ostream& os, const double panel_eff, const double panel_area) const {
  // Check if the solar panel efficiency and area are within the valid domain.
  if (!VaildateSolarPanelEfficiency(panel_eff)) {
    throw std::invalid_argument(
        "Solar panel efficiency must be a value between 0 and 1 (inclusive)");
  }
  if (!ValidateSolarPanelArea(panel_area)) {
    throw std::invalid_argument(
        "Solar panel area must be a value greater than 0");
  }

  // os << std::setprecision(3);

  // Loop over the PvData instances in the PvCollection, calculating and sending it to the output stream.
  for (const auto& [key, value] : *pv_collection_) {
    os << key << "\n"
       << PvMetrics::CalculateDailyEnergyYeild(*value, coords_, key, panel_eff,
                                               panel_area)
       << " kWh\n\n";
  }
}

inline weatherer::PvCollectionPtr weatherer::PvHandler::GetPvCollection() const {
  return pv_collection_;
}

