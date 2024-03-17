#pragma once
#include <map>
#include <memory>
#include <string>

#include <cpr/response.h>

#include "api/models/Coordinates.hpp"
#include "api/models/PvData.hpp"
#include "util/Date.hpp"

namespace weatherer {

using PvDataPtr = std::shared_ptr<PvData>;
using PvCollectionPtr = std::shared_ptr<std::unordered_map<std::string, PvDataPtr>>;

/**
 * @class PvDataProcessor
 * @brief A class responsible for processing photovoltaic (PV) data retrieved from the Open-Meteo API.
 *
 * The PvDataProcessor class provides methods to fetch current and historical weather data, retrieve
 * weather data for specific days, and generate bulk weather data for a given time frame.
 *
 * Note: This class should not typically be used directly, but rather through the PvHandler class.
 */
class PvDataProcessor {
  static constexpr std::string_view kApiUrl_{
      "https://api.open-meteo.com/v1/forecast"};
  static constexpr std::string_view kHistoricalApiUrl_{
      "https://archive-api.open-meteo.com/v1/archive"};

/**
 * @brief Fetches current weather data from the Open-Meteo API.
 * @param coords The coordinates for which weather data is to be fetched.
 * @param time_frame The time frame for which data is requested.
 * @return cpr::Response containing the HTTP response.
 * @throws std::runtime_error if an error occurs during the HTTP request or if the response status code is not 200.
 *
 * Constructs a request to the Open-Meteo API to retrieve current weather
 * data for a specified geographical location (latitude and longitude) within a given
 * time frame. The fetched data includes details such as sunrise and sunset times,
 * temperature, cloud cover, wind speed, direct and diffuse radiation.
 */
  [[nodiscard]] static cpr::Response IngestData(
      const Coordinates& coords, const util::TimeFrame& time_frame, bool historical = false);

/**
 * @brief Generates bulk weather data from the fetched JSON response.
 * @param response The JSON response containing weather data.
 * @param time_frame The time frame for which data is requested.
 * @return PvCollectionPtr containing bulk weather data.
 * @throws std::out_of_range if the expected JSON structure is not present.
 *
 * Processes the JSON response containing hourly weather data for a
 * specified time frame. It chunks the data into daily segments and creates a
 * PvCollectionPtr containing a map of dates and corresponding PvDataPtr with
 * details such as sunrise and sunset times, diffuse and direct radiation, temperature,
 * cloud cover, and wind speed for each day.
 */
  [[nodiscard]] static PvCollectionPtr OrganizeWeatherData(const std::string& response,
                                          const util::TimeFrame& time_frame);

 public:
  // Prevent instantiation of the PvDataProcessor class.
  PvDataProcessor() = delete;
  ~PvDataProcessor() = delete;

/**
 * @brief Aggregates all weather data based on the specified time frame.
 * @param coords The coordinates for which data is to be aggregated.
 * @param time_frame The time frame for which data is requested.
 * @return PvCollectionPtr containing aggregated weather data.
 *
 * Aggregates weather data based on different scenarios. If the time frame
 * covers a period starting and ending in the past, it fetches historical data and
 * generates bulk data. If the time frame starts in the past and ends in the future,
 * it combines historical and current data. If the time frame starts and ends in the
 * future, it directly fetches and generates bulk data for the future period.
 *
 * Note: This method provides additional logic for aggregating weather data compared
 * to GenerateBulkData, incorporating historical data and handling future periods.
 */
  [[nodiscard]] static PvCollectionPtr CollectData(
      const Coordinates& coords, const util::TimeFrame& time_frame);
};
}  // namespace weatherer
