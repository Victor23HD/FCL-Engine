#pragma once

#include <fcl/core/types.hpp>

#include <cstdint>

namespace fcl
{

enum class SensorQuality : std::uint8_t
{
    Valid,
    Suspect,
    Failed,
    Saturated
};

struct SensorReading
{
    float value = 0.0f;
    SensorQuality quality = SensorQuality::Valid;
    TimestampNs timestamp_ns = 0;
    std::uint32_t sequence = 0;
};

}
