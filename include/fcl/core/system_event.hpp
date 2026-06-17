#pragma once

#include <cstdint>

namespace fcl
{

enum class SystemEvent : std::uint16_t
{
    None = 0,
    HydraulicPressureDrop,
    GearStuck,
    PitotSpeedLoss,
    SensorTimeout,
    WatchdogHeartbeatLost,
    ControlModeDegraded
};

}
