#include "virtual_hydraulic_sensor.hpp"

namespace fcl
{

static constexpr std::uint16_t SAMPLE_RATE_HZ = 100;

VirtualHydraulicSensor::VirtualHydraulicSensor(SensorId id, float pressure_bar)
    : id_(id)
    , pressure_bar_(pressure_bar)
{
}

SensorId VirtualHydraulicSensor::id() const noexcept
{
    return id_;
}

SampleRateHz VirtualHydraulicSensor::sample_rate() const noexcept
{
    SampleRateHz rate;
    rate.value = SAMPLE_RATE_HZ;
    return rate;
}

Status VirtualHydraulicSensor::sample(SensorReading& out) noexcept
{
    out.value = pressure_bar_;
    out.quality = SensorQuality::Valid;
    out.sequence = ++sequence_;

    out.timestamp_ns = static_cast<TimestampNs>(sequence_) * 10000000u;

    heartbeat_++;
    return ok_status;
}

std::uint32_t VirtualHydraulicSensor::heartbeat_sequence() const noexcept
{
    return heartbeat_;
}

}
