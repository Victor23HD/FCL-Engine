#include <fcl/flight/flight_control_engine.hpp>

namespace fcl
{

void FlightControlEngine::reset() noexcept
{
    mode_ = FlightControlMode::Normal;
    last_event_ = SystemEvent::None;
}

void FlightControlEngine::update(
    Status vote_st,
    int faulty_channel,
    SystemEvent event) noexcept
{
    if (event == SystemEvent::WatchdogHeartbeatLost)
    {
        last_event_ = event;
        mode_ = FlightControlMode::Direct;
        return;
    }

    if (!vote_st.ok())
    {
        last_event_ = SystemEvent::HydraulicPressureDrop;
        mode_ = FlightControlMode::Direct;
        return;
    }

    if (faulty_channel >= 0)
    {
        last_event_ = SystemEvent::ControlModeDegraded;
        mode_ = FlightControlMode::Alternate;
        return;
    }

    last_event_ = SystemEvent::None;
    mode_ = FlightControlMode::Normal;
}

}
