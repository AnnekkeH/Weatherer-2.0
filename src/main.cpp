#include <iostream>
#include <print>
#include <memory>

#include "api/PvHandler.hpp"
#include "api/models/Coordinates.hpp"
#include "util/Time.hpp"


int main() {
  double latitute{}, longitude{}, panel_eff{}, panel_area{};
  std::print("Enter latitude (Up two decmial places): EX: 52.52\n");
  std::cin >> latitute;
  std::print("Enter longitude (Up two decmial places): EX: 13.41\n");
  std::cin >> longitude;
  std::print(
      "Enter panel efficiency (Value must be a value between 0 and 1 "
      "(inclusive)): EX: 0.95\n");
  std::cin >> panel_eff;
  std::print("Enter panel area (Value must be a value greater than 0): EX: 10\n");
  std::cin >> panel_area;



  const auto pv_handle = std::make_unique<weatherer::PvHandler>(
      weatherer::Coordinates{latitute, longitude},
      weatherer::util::TimeFrame{"2023-07-01", "2023-08-01"});

  pv_handle->OutputData(std::cout);
  pv_handle->OutputDailyEnergyYeild(std::cout, panel_eff, panel_area);

  system("pause");
  return 0;
}
