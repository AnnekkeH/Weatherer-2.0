#pragma once

#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

namespace weather {
	class NetworkUtils {
		using json = nlohmann::json;

		template <typename Key, typename... Keys, typename = std::enable_if_t<(sizeof...(Keys) == 0)>>
		static json access_json_property(const json& data, const Key& key) {
			return data.at(key);
		}

		template <typename Key, typename... Keys, typename = std::enable_if_t<(sizeof...(Keys) > 0)>>
		static json access_json_property(const json& data, const Key& key, const Keys&... keys) {
			return access_json_property(data.at(key), keys...);
		}


	public:
		// Do NOT allow an instance of this class to be created
		NetworkUtils() = delete;

		static std::string get_http_response(const std::string& uri);

		template <class... Keys, typename = std::enable_if_t<(sizeof...(Keys) > 0)>>
		static std::string get_http_data(const std::string& uri, const Keys&... properties) {
			const json data = json::parse(get_http_response(uri));
			return get_http_data(data, properties...);
		}

		template <class... Keys, typename = std::enable_if_t<(sizeof...(Keys) > 0)>>
		static std::string get_http_data(const json& json_data, const Keys&... properties) {
			return access_json_property(json_data, properties...);
		}
	};
}
