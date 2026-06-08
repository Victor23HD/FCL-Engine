#include <fcl/core/flight_control_mode.hpp>
#include <fcl/core/system_event.hpp>

#include <cstdio>

namespace
{

    const char *mode_name(fcl::FlightControlMode mode)
    {
        switch (mode)
        {
        case fcl::FlightControlMode::Normal:
            return "Normal";
        case fcl::FlightControlMode::Alternate:
            return "Alternate";
        case fcl::FlightControlMode::Direct:
            return "Direct";
        case fcl::FlightControlMode::Emergency:
            return "Emergency";
        }
        return "Unknown";
    }

}

int main()
{
    std::printf("FCL-Engine v0.1.0\n");
    std::printf("flight control mode: %s\n", mode_name(fcl::FlightControlMode::Normal));
    std::printf("system event enum: ok\n");
    return 0;
}
