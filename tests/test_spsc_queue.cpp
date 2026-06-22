#include <fcl/messaging/spsc_reading_queue.hpp>
#include <fcl/sensors/sensor_reading.hpp>

#include <gtest/gtest.h>

TEST(SpscReadingQueue, push_pop_fifo)
{
    fcl::SpscReadingQueue queue;
    fcl::SensorReading a{};
    fcl::SensorReading b{};
    fcl::SensorReading out{};

    a.value = 100.0f;
    b.value = 200.0f;

    EXPECT_TRUE(queue.push(a));
    EXPECT_TRUE(queue.push(b));
    EXPECT_FALSE(queue.empty());

    EXPECT_TRUE(queue.pop(out));
    EXPECT_FLOAT_EQ(out.value, 100.0f);

    EXPECT_TRUE(queue.pop(out));
    EXPECT_FLOAT_EQ(out.value, 200.0f);

    EXPECT_TRUE(queue.empty());
}

TEST(SpscReadingQueue, full_when_capacity_reached)
{
    fcl::SpscReadingQueue queue;
    fcl::SensorReading reading{};
    fcl::SensorReading out{};

    for (std::size_t i = 0; i < fcl::SpscReadingQueue::CAPACITY_LEN - 1; i++)
    {
        reading.value = static_cast<float>(i);
        EXPECT_TRUE(queue.push(reading));
    }

    EXPECT_TRUE(queue.full());
    EXPECT_FALSE(queue.push(reading));

    EXPECT_TRUE(queue.pop(out));
    EXPECT_FALSE(queue.full());
}
