#pragma once

#include <cstdint>

namespace fcl
{

using SensorId = std::uint8_t;
using TimestampNs = std::uint64_t;

struct SampleRateHz
{
    std::uint16_t value = 0;
};

}
