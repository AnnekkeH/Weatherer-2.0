#include "api/PvHandler.hpp"
#include <utility>
#include <chrono>
#include <iostream>

int main() {
	const auto map = 
		weatherer::PvHandler::generate_pv_data({52.52, 13.41}, "2023-12-05", "2023-12-15");

	for (const auto& [key, value] : *map) {
		std::cout << key << "\n" << *value << "\n";
	}
}
