#pragma once

#include <fcl/core/system_event.hpp>
#include <fcl/sensors/sensor_interface.hpp>

#include <atomic>
#include <cstddef>
#include <thread>

namespace fcl
{

class WatchdogMonitor
{
public:
    void start(ISensor* const* sensors, std::size_t sensor_count) noexcept;
    void stop() noexcept;
    void join() noexcept;

    SystemEvent last_event() const noexcept
    {
        return last_event_.load(std::memory_order_acquire);
    }

private:
    ISensor* const* sensors_ = nullptr;
    std::size_t sensor_count_ = 0;
    std::atomic<bool> stop_{false};
    std::atomic<SystemEvent> last_event_{SystemEvent::None};
    std::thread thread_;
};

}
