#pragma once

#include <fcl/core/status.hpp>
#include <fcl/sensors/sensor_reading.hpp>

namespace fcl
{

class TmrVoter
{
public:
    Status vote(
        const SensorReading& reading_a,
        const SensorReading& reading_b,
        const SensorReading& reading_c,
        SensorReading& out,
        int* faulty_channel_index = nullptr) noexcept;

private:
    static bool pressure_matches(float left, float right) noexcept;
};

}
