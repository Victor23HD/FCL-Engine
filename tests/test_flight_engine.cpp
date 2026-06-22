#include <fcl/core/status.hpp>
#include <fcl/core/system_event.hpp>
#include <fcl/flight/flight_control_engine.hpp>

#include <gtest/gtest.h>

TEST(FlightControlEngine, normal_when_vote_ok)
{
    fcl::FlightControlEngine engine;
    engine.update(fcl::ok_status, -1, fcl::SystemEvent::None);

    EXPECT_EQ(engine.mode(), fcl::FlightControlMode::Normal);
    EXPECT_EQ(engine.last_event(), fcl::SystemEvent::None);
}

TEST(FlightControlEngine, alternate_on_faulty_channel)
{
    fcl::FlightControlEngine engine;
    engine.update(fcl::ok_status, 2, fcl::SystemEvent::None);

    EXPECT_EQ(engine.mode(), fcl::FlightControlMode::Alternate);
    EXPECT_EQ(engine.last_event(), fcl::SystemEvent::ControlModeDegraded);
}

TEST(FlightControlEngine, direct_on_vote_fail)
{
    fcl::FlightControlEngine engine;
    fcl::Status fault{fcl::StatusCode::Fault};

    engine.update(fault, -1, fcl::SystemEvent::None);

    EXPECT_EQ(engine.mode(), fcl::FlightControlMode::Direct);
    EXPECT_EQ(engine.last_event(), fcl::SystemEvent::HydraulicPressureDrop);
}

TEST(FlightControlEngine, direct_on_watchdog)
{
    fcl::FlightControlEngine engine;
    engine.update(fcl::ok_status, -1, fcl::SystemEvent::WatchdogHeartbeatLost);

    EXPECT_EQ(engine.mode(), fcl::FlightControlMode::Direct);
    EXPECT_EQ(engine.last_event(), fcl::SystemEvent::WatchdogHeartbeatLost);
}
