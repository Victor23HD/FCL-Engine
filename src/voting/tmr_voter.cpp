#include <fcl/voting/tmr_voter.hpp>

namespace fcl
{

static constexpr float PRESSURE_TOLERANCE_BAR = 1.0f;

bool TmrVoter::pressure_matches(float left, float right) noexcept
{
    float diff = left - right;
    if (diff < 0.0f)
    {
        diff = -diff;
    }
    return diff <= PRESSURE_TOLERANCE_BAR;
}

Status TmrVoter::vote(
    const SensorReading& reading_a,
    const SensorReading& reading_b,
    const SensorReading& reading_c,
    SensorReading& out,
    int* faulty_channel_index) noexcept
{
    if (faulty_channel_index != nullptr)
    {
        *faulty_channel_index = -1;
    }

    const bool ab_match = pressure_matches(reading_a.value, reading_b.value);
    const bool ac_match = pressure_matches(reading_a.value, reading_c.value);
    const bool bc_match = pressure_matches(reading_b.value, reading_c.value);

    if (ab_match && !ac_match)
    {
        out = reading_a;
        if (faulty_channel_index != nullptr)
        {
            *faulty_channel_index = 2;
        }
        return ok_status;
    }

    if (ac_match && !ab_match)
    {
        out = reading_a;
        if (faulty_channel_index != nullptr)
        {
            *faulty_channel_index = 1;
        }
        return ok_status;
    }

    if (bc_match && !ab_match)
    {
        out = reading_b;
        if (faulty_channel_index != nullptr)
        {
            *faulty_channel_index = 0;
        }
        return ok_status;
    }

    if (ab_match && ac_match)
    {
        out = reading_a;
        return ok_status;
    }

    return Status{StatusCode::Fault};
}

}
