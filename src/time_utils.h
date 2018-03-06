#pragma once
#include <ctime>
#include <string>
#include <chrono>

namespace TimeUtils
{
    std::string getCurrentTimeForFileName()
    {
        auto now = std::chrono::system_clock::now();
        const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        struct tm * t = localtime( &nowTime );
        char* buffer = new char[64];
        strftime(buffer, 64, "%Y-%m-%d-%H-%M-%S", t);
        return std::string(buffer);
    }

    std::string getUnixTimeAsString()
    {
        std::time_t unixTime = std::time(nullptr);
        return std::to_string(unixTime);
    }

}