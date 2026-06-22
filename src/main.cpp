#include <fcl/core/flight_control_mode_label.hpp>
#include <fcl/core/status.hpp>
#include <fcl/core/system_event_label.hpp>
#include <fcl/flight/flight_control_engine.hpp>
#include <fcl/messaging/spsc_reading_queue.hpp>
#include <fcl/sensors/quality_label.hpp>
#include <fcl/sensors/sensor_reading.hpp>
#include <fcl/voting/tmr_voter.hpp>
#include <fcl/watchdog/watchdog_monitor.hpp>

#include <sensor_worker.hpp>
#include <virtual_hydraulic_sensor.hpp>

#include <cstdio>
#include <chrono>
#include <thread>

namespace
{

static constexpr float FAULTY_PRESSURE_BAR = 2500.0f;
static constexpr int BURST_SAMPLES = 5;
static constexpr int BURST_INTERVAL_MS = 20;

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

static void drain_spsc(fcl::SpscReadingQueue& queue, const char* label)
{
    fcl::SensorReading reading{};
    int pop_count = 0;

    std::printf("--- drain %s ---\n", label);

    while (queue.pop(reading))
    {
        pop_count++;
        std::printf("[pop %d] ", pop_count);
        print_reading(reading);
    }

    if (pop_count == 0)
    {
        std::printf("(empty)\n");
    }
}

static void run_tmr_cycle()
{
    std::printf("=== cycle 1: TMR pressure fault ===\n\n");

    fcl::VirtualHydraulicSensor sensor_a(1);
    fcl::VirtualHydraulicSensor sensor_b(2);
    fcl::VirtualHydraulicSensor sensor_c(3, FAULTY_PRESSURE_BAR);

    fcl::ISensor* sensor_channel[3];
    sensor_channel[0] = &sensor_a;
    sensor_channel[1] = &sensor_b;
    sensor_channel[2] = &sensor_c;

    fcl::SpscReadingQueue channel_queue[3];
    fcl::SensorReading channel_readings[3]{};
    fcl::SensorReading voted_reading{};
    fcl::TmrVoter voter;
    fcl::FlightControlEngine engine;
    int faulty_channel = -1;

    std::printf("3 sensor threads, SPSC queue per channel\n\n");
    std::printf("--- sample (threads) ---\n");

    std::thread thread_a(fcl::run_one_sample,
        std::ref(sensor_a),
        std::ref(channel_readings[0]),
        std::ref(channel_queue[0]));

    std::thread thread_b(fcl::run_one_sample,
        std::ref(sensor_b),
        std::ref(channel_readings[1]),
        std::ref(channel_queue[1]));

    std::thread thread_c(fcl::run_one_sample,
        std::ref(sensor_c),
        std::ref(channel_readings[2]),
        std::ref(channel_queue[2]));

    thread_a.join();
    thread_b.join();
    thread_c.join();

    for (int i = 0; i < 3; i++)
    {
        std::printf("[sensor %u] ", sensor_channel[i]->id());
        print_reading(channel_readings[i]);
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
    }

    engine.update(vote_st, faulty_channel, fcl::SystemEvent::None);
    std::printf("flight control mode: %s\n", fcl::flight_control_mode_label(engine.mode()));
    std::printf("system event: %s\n\n", fcl::system_event_label(engine.last_event()));

    for (int i = 0; i < 3; i++)
    {
        char label[16];
        std::snprintf(label, sizeof(label), "ch-%u", sensor_channel[i]->id());
        drain_spsc(channel_queue[i], label);
    }
}

static void run_watchdog_cycle()
{
    std::printf("\n=== cycle 2: watchdog heartbeat fault ===\n\n");

    fcl::VirtualHydraulicSensor sensor_a(1);
    fcl::VirtualHydraulicSensor sensor_b(2);
    fcl::VirtualHydraulicSensor sensor_c(3);

    sensor_b.set_freeze_heartbeat(true);

    fcl::ISensor* sensor_channel[3];
    sensor_channel[0] = &sensor_a;
    sensor_channel[1] = &sensor_b;
    sensor_channel[2] = &sensor_c;

    fcl::SpscReadingQueue channel_queue[3];
    fcl::SensorReading channel_readings[3]{};
    fcl::SensorReading voted_reading{};
    fcl::TmrVoter voter;
    fcl::FlightControlEngine engine;
    fcl::WatchdogMonitor watchdog;
    int faulty_channel = -1;

    std::printf("sensor 2: freeze heartbeat (fault inject)\n");
    std::printf("burst: %d samples x %d ms, watchdog polling\n\n", BURST_SAMPLES, BURST_INTERVAL_MS);

    watchdog.start(sensor_channel, 3);

    std::thread thread_a(fcl::run_sample_burst,
        std::ref(sensor_a),
        std::ref(channel_readings[0]),
        std::ref(channel_queue[0]),
        BURST_SAMPLES,
        std::chrono::milliseconds(BURST_INTERVAL_MS));

    std::thread thread_b([&]() {
        for (int i = 0; i < BURST_SAMPLES; i++)
        {
            sensor_b.sample(channel_readings[1]);
            channel_queue[1].push(channel_readings[1]);

            if (i + 1 < BURST_SAMPLES)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(BURST_INTERVAL_MS));
            }
        }
    });

    std::thread thread_c(fcl::run_sample_burst,
        std::ref(sensor_c),
        std::ref(channel_readings[2]),
        std::ref(channel_queue[2]),
        BURST_SAMPLES,
        std::chrono::milliseconds(BURST_INTERVAL_MS));

    thread_a.join();
    thread_b.join();
    thread_c.join();

    watchdog.stop();
    watchdog.join();

    fcl::SystemEvent wd_event = watchdog.last_event();
    std::printf("watchdog event: %s\n", fcl::system_event_label(wd_event));

    std::printf("\n--- tmr vote (latest burst) ---\n");
    fcl::Status vote_st = voter.vote(
        channel_readings[0],
        channel_readings[1],
        channel_readings[2],
        voted_reading,
        &faulty_channel);

    if (vote_st.ok())
    {
        std::printf("voted ");
        print_reading(voted_reading);
    }

    engine.update(vote_st, faulty_channel, wd_event);
    std::printf("flight control mode: %s\n", fcl::flight_control_mode_label(engine.mode()));
    std::printf("system event: %s\n", fcl::system_event_label(engine.last_event()));
}

}

int main()
{
    print_banner();
    run_tmr_cycle();
    run_watchdog_cycle();
    std::printf("\nEnd.\n");
    return 0;
}
