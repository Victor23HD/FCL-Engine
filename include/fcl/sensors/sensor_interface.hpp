#pragma once

#include <fcl/core/status.hpp>
#include <fcl/core/types.hpp>
#include <fcl/sensors/sensor_reading.hpp>

#include <cstdint>

namespace fcl
{

class ISensor
{
public:
    virtual ~ISensor() = default;

    [[nodiscard]] virtual SensorId id() const noexcept = 0;
    [[nodiscard]] virtual SampleRateHz sample_rate() const noexcept = 0;
    [[nodiscard]] virtual Status sample(SensorReading& out) noexcept = 0;
    [[nodiscard]] virtual std::uint32_t heartbeat_sequence() const noexcept = 0;
};

}
