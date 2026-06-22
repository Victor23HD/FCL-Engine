#pragma once

#include <fcl/core/flight_control_mode.hpp>
#include <fcl/core/status.hpp>
#include <fcl/core/system_event.hpp>

namespace fcl
{

class FlightControlEngine
{
public:
    FlightControlMode mode() const noexcept
    {
        return mode_;
    }

    SystemEvent last_event() const noexcept
    {
        return last_event_;
    }

    void reset() noexcept;

    void update(Status vote_st, int faulty_channel, SystemEvent event) noexcept;

private:
    FlightControlMode mode_ = FlightControlMode::Normal;
    SystemEvent last_event_ = SystemEvent::None;
};

}
