#pragma once

#include <fcl/core/flight_control_mode.hpp>

namespace fcl
{

inline const char* flight_control_mode_label(FlightControlMode mode)
{
    switch (mode)
    {
    case FlightControlMode::Normal:
        return "Normal";
    case FlightControlMode::Alternate:
        return "Alternate";
    case FlightControlMode::Direct:
        return "Direct";
    case FlightControlMode::Emergency:
        return "Emergency";
    }
    return "Unknown";
}

}
