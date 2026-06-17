#include <fcl/core/flight_control_mode.hpp>
#include <fcl/core/flight_control_mode_label.hpp>
#include <fcl/core/status.hpp>
#include <fcl/messaging/sensor_reading_queue.hpp>
#include <fcl/sensors/quality_label.hpp>
#include <fcl/sensors/sensor_interface.hpp>
#include <fcl/sensors/sensor_reading.hpp>
#include <fcl/voting/tmr_voter.hpp>
#include <fcl/watchdog/heartbeat_check.hpp>

#include <virtual_hydraulic_sensor.hpp>

#include <cstdio>

namespace
{

static constexpr float FAULTY_PRESSURE_BAR = 2500.0f;

static void print_reading(const fcl::SensorReading& reading)
{
    std::printf("pressure=%.1f bar seq=%u quality=%s\n",
        reading.value,
        reading.sequence,
        FCL_QUALITY_LABEL(reading.quality));
}

static void print_banner()
{
    std::printf("FCL-Engine v0.1.0\n");
    std::printf("Flight Control Laws | critical onboard software simulator\n\n");
}

static void sample_channel(
    fcl::ISensor& sensor,
    fcl::SensorReading& out,
    fcl::SensorReadingQueue& queue)
{
    std::uint32_t hb_before = sensor.heartbeat_sequence();
    fcl::Status st = sensor.sample(out);

    if (!st.ok())
    {
        std::printf("[sensor %u] sample failed\n", sensor.id());
        return;
    }

    if (!fcl::check_heartbeat(sensor, hb_before).ok())
    {
        std::printf("[sensor %u] watchdog: heartbeat stuck\n", sensor.id());
        return;
    }

    std::printf("[sensor %u] ", sensor.id());
    print_reading(out);

    if (queue.full())
    {
        std::printf("[sensor %u] queue-full\n", sensor.id());
        return;
    }

    queue.push(out);
    std::printf("[sensor %u] pushed queue-size=%zu\n", sensor.id(), queue.size());
}

static fcl::FlightControlMode mode_from_vote(fcl::Status vote_st)
{
    if (vote_st.ok())
    {
        return fcl::FlightControlMode::Normal;
    }
    return fcl::FlightControlMode::Alternate;
}

static void drain_queue(fcl::SensorReadingQueue& queue, fcl::SensorReading& reading)
{
    std::printf("\n--- empty queue ---\n");

    for (int i = 0; i < 3; i++)
    {
        if (queue.empty())
        {
            std::printf("[pop %d] empty queue\n", i + 1);
            break;
        }

        fcl::Status st = queue.pop(reading);

        if (!st.ok())
        {
            std::printf("[pop %d] pop failed\n", i + 1);
            continue;
        }

        std::printf("[pop %d] ", i + 1);
        print_reading(reading);
        std::printf("           queue-size=%zu\n", queue.size());
    }
}

static void run_smoke_cycle()
{
    fcl::VirtualHydraulicSensor sensor_a(1);
    fcl::VirtualHydraulicSensor sensor_b(2);
    fcl::VirtualHydraulicSensor sensor_c(3, FAULTY_PRESSURE_BAR);

    fcl::ISensor* sensor_channel[3];
    sensor_channel[0] = &sensor_a;
    sensor_channel[1] = &sensor_b;
    sensor_channel[2] = &sensor_c;

    fcl::SensorReading channel_readings[3]{};
    fcl::SensorReading voted_reading{};
    fcl::SensorReading reading{};
    fcl::SensorReadingQueue queue;
    fcl::TmrVoter voter;
    int faulty_channel = -1;

    std::printf("channels: 3 sensors (C faulty), TMR vote, queue\n");
    std::printf("queue cap: %zu\n\n", fcl::SensorReadingQueue::CAPACITY_LEN);

    std::printf("--- sample sensors ---\n");
    for (int i = 0; i < 3; i++)
    {
        sample_channel(*sensor_channel[i], channel_readings[i], queue);
    }

    std::printf("\n--- tmr vote ---\n");
    fcl::Status vote_st = voter.vote(
        channel_readings[0],
        channel_readings[1],
        channel_readings[2],
        voted_reading,
        &faulty_channel);

    if (!vote_st.ok())
    {
        std::printf("tmr vote failed (no majority)\n");
    }
    else
    {
        std::printf("voted ");
        print_reading(voted_reading);
        if (faulty_channel >= 0)
        {
            std::printf("faulty channel index: %d (sensor %u)\n",
                faulty_channel,
                sensor_channel[faulty_channel]->id());
        }
        else
        {
            std::printf("all channels agree\n");
        }
    }

    fcl::FlightControlMode mode = mode_from_vote(vote_st);
    std::printf("flight control mode: %s\n", fcl::flight_control_mode_label(mode));

    drain_queue(queue, reading);
    std::printf("\nEnd. queue-size=%zu\n", queue.size());
}

}

int main()
{
    print_banner();
    run_smoke_cycle();
    return 0;
}
