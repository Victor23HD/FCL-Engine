#pragma once

#include <fcl/sensors/sensor_interface.hpp>

namespace fcl
{

class VirtualHydraulicSensor : public ISensor
{
public:
    static constexpr float NOMINAL_PRESSURE_BAR = 3000.0f;

    VirtualHydraulicSensor(SensorId id, float pressure_bar = NOMINAL_PRESSURE_BAR);

    SensorId id() const noexcept override;
    SampleRateHz sample_rate() const noexcept override;
    Status sample(SensorReading& out) noexcept override;
    std::uint32_t heartbeat_sequence() const noexcept override;

private:
    SensorId id_;
    float pressure_bar_;
    std::uint32_t heartbeat_ = 0;
    std::uint32_t sequence_ = 0;
};

}
