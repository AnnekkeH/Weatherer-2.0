#pragma once

#include "api/PvDataProcessor.hpp"
#include "api/models/Coordinates.hpp"
#include "util/Time.hpp"

namespace weatherer {
class PvHandler {
 private:
  Coordinates coords_;
  util::TimeFrame time_frame_;
  PvCollectionPtr pv_collection_;

  [[nodiscard]] inline static bool VaildateSolarPanelEfficiency(const double panel_eff);
  [[nodiscard]] inline static bool VaildateSolarPanelArea(const double panel_area);

 public:
  [[nodiscard]] explicit PvHandler(Coordinates const& coords,
                          util::TimeFrame const& time_frame);
  ~PvHandler();
  PvHandler(const PvHandler& other);
  PvHandler(PvHandler&& other) noexcept;
  PvHandler& operator=(const PvHandler& other);
  PvHandler& operator=(PvHandler&& other) noexcept;

  // void PrintData() const;
  void OutputData(std::ostream& os) const;
  // void PrintDailyEnergyYeild(const double panel_eff,
                                 // const double panel_area) const;
  void OutputDailyEnergyYeild(std::ostream& os,
                                 const double panel_eff,
                                 const double panel_area) const;

  [[nodiscard]] inline PvCollectionPtr GetPvCollection() const;

};
}  // namespace weatherer
