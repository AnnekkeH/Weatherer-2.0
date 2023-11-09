#ifndef FOXTROT_WEATHERPROVIDER_HPP
#define FOXTROT_WEATHERPROVIDER_HPP

#include <string>

namespace weather {
    class WeatherProvider {
    public:
        // Do NOT allow an instance of this class to be created
        WeatherProvider() = delete;

        [[nodiscard]]
        static std::string get_data(const std::string& location) noexcept;
    };
}


#endif //FOXTROT_WEATHERPROVIDER_HPP