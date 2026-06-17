#pragma once

#include <fcl/sensors/sensor_reading.hpp>

namespace fcl
{

inline const char* quality_label(SensorQuality quality)
{
    switch (quality)
    {
    case SensorQuality::Valid:
        return "Ok";
    case SensorQuality::Suspect:
        return "Suspect";
    case SensorQuality::Failed:
        return "Failed";
    case SensorQuality::Saturated:
        return "Saturated";
    }
    return "Unknown";
}

}

#define FCL_QUALITY_LABEL(quality) fcl::quality_label((quality))
