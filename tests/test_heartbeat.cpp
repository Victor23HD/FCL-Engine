#include <fcl/core/status.hpp>
#include <fcl/sensors/sensor_reading.hpp>
#include <fcl/watchdog/heartbeat_check.hpp>

#include <virtual_hydraulic_sensor.hpp>

#include <gtest/gtest.h>

TEST(HeartbeatCheck, bumps_after_sample)
{
    fcl::VirtualHydraulicSensor sensor(1);
    fcl::SensorReading reading{};

    std::uint32_t hb_before = sensor.heartbeat_sequence();
    ASSERT_TRUE(sensor.sample(reading).ok());

    fcl::Status st = fcl::check_heartbeat(sensor, hb_before);
    EXPECT_TRUE(st.ok());
}

TEST(HeartbeatCheck, fails_when_frozen)
{
    fcl::VirtualHydraulicSensor sensor(2);
    fcl::SensorReading reading{};

    sensor.set_freeze_heartbeat(true);

    std::uint32_t hb_before = sensor.heartbeat_sequence();
    ASSERT_TRUE(sensor.sample(reading).ok());

    fcl::Status st = fcl::check_heartbeat(sensor, hb_before);
    EXPECT_FALSE(st.ok());
}
