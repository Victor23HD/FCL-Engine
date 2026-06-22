#include <fcl/sensors/sensor_reading.hpp>
#include <fcl/voting/tmr_voter.hpp>

#include <gtest/gtest.h>

namespace
{

static fcl::SensorReading make_reading(float pressure)
{
    fcl::SensorReading reading{};
    reading.value = pressure;
    reading.quality = fcl::SensorQuality::Valid;
    return reading;
}

}

TEST(TmrVoter, two_agree_one_faulty)
{
    fcl::TmrVoter voter;
    fcl::SensorReading out{};
    int faulty = -1;

    fcl::Status st = voter.vote(
        make_reading(3000.0f),
        make_reading(3000.0f),
        make_reading(2500.0f),
        out,
        &faulty);

    EXPECT_TRUE(st.ok());
    EXPECT_FLOAT_EQ(out.value, 3000.0f);
    EXPECT_EQ(faulty, 2);
}

TEST(TmrVoter, all_agree)
{
    fcl::TmrVoter voter;
    fcl::SensorReading out{};
    int faulty = -1;

    fcl::Status st = voter.vote(
        make_reading(3000.0f),
        make_reading(3000.0f),
        make_reading(3000.0f),
        out,
        &faulty);

    EXPECT_TRUE(st.ok());
    EXPECT_EQ(faulty, -1);
}

TEST(TmrVoter, no_majority)
{
    fcl::TmrVoter voter;
    fcl::SensorReading out{};

    fcl::Status st = voter.vote(
        make_reading(3000.0f),
        make_reading(2500.0f),
        make_reading(2000.0f),
        out,
        nullptr);

    EXPECT_FALSE(st.ok());
}
