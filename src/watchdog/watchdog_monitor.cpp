#include <fcl/watchdog/watchdog_monitor.hpp>

#include <array>
#include <chrono>
#include <thread>

namespace fcl
{

namespace
{

static constexpr int STUCK_THRESHOLD = 3;
static constexpr int POLL_MS = 10;

void watchdog_loop(
    ISensor* const* sensors,
    std::size_t sensor_count,
    std::atomic<bool>& stop,
    std::atomic<SystemEvent>& last_event)
{
    std::array<std::uint32_t, 8> last_hb{};
    std::array<int, 8> stuck_ticks{};

    for (std::size_t i = 0; i < sensor_count; i++)
    {
        last_hb[i] = sensors[i]->heartbeat_sequence();
    }

    while (!stop.load(std::memory_order_acquire))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(POLL_MS));

        for (std::size_t i = 0; i < sensor_count; i++)
        {
            std::uint32_t hb = sensors[i]->heartbeat_sequence();

            if (hb == last_hb[i])
            {
                stuck_ticks[i]++;
                if (stuck_ticks[i] >= STUCK_THRESHOLD)
                {
                    last_event.store(SystemEvent::WatchdogHeartbeatLost, std::memory_order_release);
                }
            }
            else
            {
                stuck_ticks[i] = 0;
                last_hb[i] = hb;
            }
        }
    }
}

}

void WatchdogMonitor::start(ISensor* const* sensors, std::size_t sensor_count) noexcept
{
    sensors_ = sensors;
    sensor_count_ = sensor_count;
    stop_.store(false, std::memory_order_release);
    last_event_.store(SystemEvent::None, std::memory_order_release);

    thread_ = std::thread(
        watchdog_loop,
        sensors_,
        sensor_count_,
        std::ref(stop_),
        std::ref(last_event_));
}

void WatchdogMonitor::stop() noexcept
{
    stop_.store(true, std::memory_order_release);
}

void WatchdogMonitor::join() noexcept
{
    if (thread_.joinable())
    {
        thread_.join();
    }
}

}
