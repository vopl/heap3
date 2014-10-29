#pragma once

#include "mm/utils/sized_cast.hpp"

namespace mm { namespace utils
{
    template <typename To, typename From>
    To sized_cast(From from)
    {
        static_assert(sizeof(To) == sizeof(From), "operands must be same size for sized_cast");
        return reinterpret_cast<To>(from);
    }
}}
