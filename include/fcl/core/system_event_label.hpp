#pragma once

#include <fcl/core/system_event.hpp>

namespace fcl
{

inline const char* system_event_label(SystemEvent event)
{
    switch (event)
    {
    case SystemEvent::None:
        return "None";
    case SystemEvent::HydraulicPressureDrop:
        return "HydraulicPressureDrop";
    case SystemEvent::GearStuck:
        return "GearStuck";
    case SystemEvent::PitotSpeedLoss:
        return "PitotSpeedLoss";
    case SystemEvent::SensorTimeout:
        return "SensorTimeout";
    case SystemEvent::WatchdogHeartbeatLost:
        return "WatchdogHeartbeatLost";
    case SystemEvent::ControlModeDegraded:
        return "ControlModeDegraded";
    }
    return "Unknown";
}

}
