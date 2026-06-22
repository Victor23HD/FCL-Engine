#pragma once

#include <fcl/sensors/sensor_reading.hpp>

#include <array>
#include <atomic>
#include <cstddef>

namespace fcl
{

class SpscReadingQueue
{
public:
    static constexpr std::size_t CAPACITY_LEN = 8;

    bool full() const noexcept
    {
        std::size_t head = szhead.load(std::memory_order_relaxed);
        std::size_t tail = sztail.load(std::memory_order_acquire);
        std::size_t next = (head + 1) % CAPACITY_LEN;
        return next == tail;
    }

    bool empty() const noexcept
    {
        std::size_t tail = sztail.load(std::memory_order_relaxed);
        std::size_t head = szhead.load(std::memory_order_acquire);
        return tail == head;
    }

    bool push(const SensorReading& reading) noexcept
    {
        std::size_t head = szhead.load(std::memory_order_relaxed);
        std::size_t next = (head + 1) % CAPACITY_LEN;

        if (next == sztail.load(std::memory_order_acquire))
        {
            return false;
        }

        buffer[head] = reading;
        szhead.store(next, std::memory_order_release);
        return true;
    }

    bool pop(SensorReading& out) noexcept
    {
        std::size_t tail = sztail.load(std::memory_order_relaxed);

        if (tail == szhead.load(std::memory_order_acquire))
        {
            return false;
        }

        out = buffer[tail];
        sztail.store((tail + 1) % CAPACITY_LEN, std::memory_order_release);
        return true;
    }

    std::size_t size_approx() const noexcept
    {
        std::size_t head = szhead.load(std::memory_order_acquire);
        std::size_t tail = sztail.load(std::memory_order_acquire);

        if (head >= tail)
        {
            return head - tail;
        }
        return CAPACITY_LEN - tail + head;
    }

private:
    std::array<SensorReading, CAPACITY_LEN> buffer{};
    std::atomic<std::size_t> szhead{0};
    std::atomic<std::size_t> sztail{0};
};

}
