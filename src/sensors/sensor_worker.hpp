#pragma once

#include <fcl/core/status.hpp>
#include <fcl/messaging/spsc_reading_queue.hpp>
#include <fcl/sensors/sensor_interface.hpp>
#include <fcl/sensors/sensor_reading.hpp>

#include <chrono>
#include <cstdint>

namespace fcl
{

Status run_one_sample(
    ISensor& sensor,
    SensorReading& out,
    SpscReadingQueue& queue) noexcept;

Status run_sample_burst(
    ISensor& sensor,
    SensorReading& latest_out,
    SpscReadingQueue& queue,
    int sample_count,
    std::chrono::milliseconds interval) noexcept;

}
