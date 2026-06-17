#pragma once

#include <fcl/core/status.hpp>
#include <fcl/sensors/sensor_reading.hpp>

#include <array>
#include <cstddef>

namespace fcl
{

class SensorReadingQueue
{
public:
    static constexpr std::size_t CAPACITY_LEN = 8;

    bool full() const
    {
        return szcount >= CAPACITY_LEN;
    }

    bool empty() const
    {
        return szcount == 0;
    }

    std::size_t size() const
    {
        return szcount;
    }

    Status push(const SensorReading& reading)
    {
        if (full())
        {
            return Status{StatusCode::BufferFull};
        }

        buffer[szhead] = reading;
        szhead = (szhead + 1) % CAPACITY_LEN;
        szcount++;
        return ok_status;
    }

    Status pop(SensorReading& out)
    {
        if (empty())
        {
            return Status{StatusCode::InvalidArgument};
        }

        out = buffer[sztail];
        sztail = (sztail + 1) % CAPACITY_LEN;
        szcount--;
        return ok_status;
    }

private:
    std::array<SensorReading, CAPACITY_LEN> buffer{};
    std::size_t szhead = 0;
    std::size_t sztail = 0;
    std::size_t szcount = 0;
};

}
