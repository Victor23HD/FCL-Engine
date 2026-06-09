#pragma once

#include <cstdint>

namespace fcl
{

    enum class StatusCode : std::uint8_t
    {
        Ok = 0,
        InvalidArgument,
        Timeout,
        Fault,
        BufferFull
    };

    struct Status
    {
        StatusCode code{StatusCode::Ok};

        [[nodiscard]] constexpr bool ok() const noexcept
        {
            return code == StatusCode::Ok;
        }

        [[nodiscard]] static constexpr Status from(StatusCode c) noexcept
        {
            return Status{c};
        }
    };

    inline constexpr Status ok_status{StatusCode::Ok};

}

