#pragma once

#include <cstdint>

namespace fcl
{

    enum class FlightControlMode : std::uint8_t
    {
        Normal,
        Alternate,
        Direct,
        Emergency
    };

    [[nodiscard]] constexpr bool is_degraded(FlightControlMode mode) noexcept
    {
        return mode != FlightControlMode::Normal;
    }
}
