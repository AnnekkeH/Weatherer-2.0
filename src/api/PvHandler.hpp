#pragma once
#include <cpr/response.h>

#include <map>

#include <memory>

#include <string>

#include "api/models/Coordinates.hpp"

#include "api/models/PvData.hpp"

#include "util/Time.hpp"

namespace weatherer {

using PvDataPtr = std::shared_ptr<PvData>;
using BulkPvDataPtr = std::shared_ptr<std::map<std::string, PvDataPtr>>;

class PvHandler {
  static constexpr std::string_view api_url_{
      "https://api.open-meteo.com/v1/forecast"};
  static constexpr std::string_view historical_api_url_{
      "https://archive-api.open-meteo.com/v1/archive"};

  [[nodiscard]] static cpr::Response GetHttpData(const Coordinates& coords,
                                                 const std::string& date);

  [[nodiscard]] static cpr::Response GetHttpData(
      const Coordinates& coords, const util::TimeFrame& time_frame);

  [[nodiscard]] static cpr::Response GetHistoricalHttpData(
      const Coordinates& coords, const std::string& date);

  [[nodiscard]] static cpr::Response GetHistoricalHttpData(
      const Coordinates& coords, const util::TimeFrame& time_frame);

  static BulkPvDataPtr Generate(const std::string& response,
                                const util::TimeFrame& time_frame);

 public:
  [[nodiscard]] static PvDataPtr GetData(const Coordinates& coords,
                                         const std::string& date);

  [[nodiscard]] static BulkPvDataPtr AggregateData(
      const Coordinates& coords, const util::TimeFrame& time_frame);
};

}  // namespace weatherer
