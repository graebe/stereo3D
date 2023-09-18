#pragma once

#include <chrono>
#include <string>

namespace Image
{
    struct ImageMetadata
    {
        std::string name{0};
        std::chrono::time_point<std::chrono::steady_clock> t{std::chrono::steady_clock::time_point()};
    };

    template <typename T>
    struct Image
    {
        T img;
        ImageMetadata meta;
    };
}