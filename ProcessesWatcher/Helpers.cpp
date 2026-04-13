#include "Helpers.hpp"

#include <iostream>
#include <chrono>

namespace FG
{
    namespace helpers
    {
        std::wstring GetCurrentFormattedTime()
        {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::tm tm;
            localtime_s(&tm, &time_t);

            std::wostringstream wss;
            wss << std::put_time(&tm, L"%H:%M:%S");
            return wss.str();
        }
    }
}
