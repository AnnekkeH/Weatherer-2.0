#pragma once

#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

namespace weather {
	class NetworkUtils {
		using json = nlohmann::json;

		// Accesses a JSON property with a single key
		template <typename Key, typename... Keys, typename = std::enable_if_t<(sizeof...(Keys) == 0)>>
		static json access_json_property(const json& data, const Key& key) {
			return data.at(key);
		}

		// Accesses a JSON property with multiple keys
		template <typename Key, typename... Keys, typename = std::enable_if_t<(sizeof...(Keys) > 0)>>
			static json access_json_property(const json& data, const Key& key, const Keys&... keys) {
			return access_json_property(data.at(key), keys...);
		}


	public:
		// Do NOT allow an instance of this class to be created
		NetworkUtils() = delete;
		
		/**
		* Sends an HTTP GET request to the specified URI and returns the response as a string
		* @param uri: The URI to send the GET request to
		* @return: The response as a string
		*/
		static std::string get_http_response(const std::string& uri);

		/** 
		* Parses the JSON data from an HTTP GET request to the specified URI and returns the value of the specified properties as a string
		* @param uri: The URI to send the GET request to
		* @param properties: The properties to extract from the JSON data
		* @return: The value of the specified properties as a string
		*/
		template <class... Keys, typename = std::enable_if_t<(sizeof...(Keys) > 0)>>
		static std::string get_http_data(const std::string& uri, const Keys&... properties) {
			const json data = json::parse(get_http_response(uri));
			return get_http_data(data, properties...);
		}

		/** 
		* Returns the value of the specified properties from the parsed JSON data
		* @param json_data: The parsed JSON data
		* @param properties: The properties to extract from the JSON data
		* @return: The value of the specified properties as a string
		*/
		template <class... Keys, typename = std::enable_if_t<(sizeof...(Keys) > 0)>>
		static std::string get_http_data(const json& json_data, const Keys&... properties) {
			return access_json_property(json_data, properties...);
		}
	};
}
