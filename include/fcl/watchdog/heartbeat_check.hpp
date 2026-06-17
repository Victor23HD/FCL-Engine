#pragma once

#include <fcl/core/status.hpp>
#include <fcl/sensors/sensor_interface.hpp>

namespace fcl
{

inline Status check_heartbeat(ISensor& sensor, std::uint32_t heartbeat_before) noexcept
{
    if (sensor.heartbeat_sequence() != heartbeat_before + 1)
    {
        return Status{StatusCode::Fault};
    }
    return ok_status;
}

}
