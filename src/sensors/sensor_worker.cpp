#include "sensor_worker.hpp"

#include <fcl/watchdog/heartbeat_check.hpp>

#include <thread>

namespace fcl
{

Status run_one_sample(
    ISensor& sensor,
    SensorReading& out,
    SpscReadingQueue& queue) noexcept
{
    std::uint32_t hb_before = sensor.heartbeat_sequence();
    Status st = sensor.sample(out);

    if (!st.ok())
    {
        return st;
    }

    st = check_heartbeat(sensor, hb_before);
    if (!st.ok())
    {
        return st;
    }

    if (queue.full())
    {
        return Status{StatusCode::BufferFull};
    }

    if (!queue.push(out))
    {
        return Status{StatusCode::BufferFull};
    }

    return ok_status;
}

Status run_sample_burst(
    ISensor& sensor,
    SensorReading& latest_out,
    SpscReadingQueue& queue,
    int sample_count,
    std::chrono::milliseconds interval) noexcept
{
    Status last = ok_status;

    for (int i = 0; i < sample_count; i++)
    {
        last = run_one_sample(sensor, latest_out, queue);

        if (!last.ok())
        {
            return last;
        }

        if (i + 1 < sample_count)
        {
            std::this_thread::sleep_for(interval);
        }
    }

    return last;
}

}
