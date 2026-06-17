# FCL-Engine

C++20 smoke-test simulator for flight control law degradation: three hydraulic pressure channels (TMR), a simple reading queue, heartbeat check, and mode selection (Normal / Alternate).

Not production avionics — a learning/portfolio project toward embedded targets (Docker x86, Raspberry Pi later).

## What runs today

Build with CMake 3.20+:

```bash
cmake -B build
cmake --build build
./build/Debug/fcl_hello    # Windows: build\Debug\fcl_hello.exe
```

The demo samples three virtual sensors (channel C injected faulty), runs 2-of-3 TMR voting, maps vote result to a flight control mode, drains the queue, and prints logs.

## Layout

```text
include/fcl/     headers (core, sensors, messaging, voting, watchdog)
src/             main, virtual_hydraulic_sensor, tmr_voter
```

## References

Material I read while shaping this code (links verified manually):

### Flight control & FBW

- [Skybrary — Flight Control Laws](https://skybrary.aero/articles/flight-control-laws)
- [Airbus — Safety innovation #1: Fly-by-wire (FBW)](https://www.airbus.com/en/newsroom/stories/2022-06-safety-innovation-1-fly-by-wire-fbw)

### Redundancy, watchdog, messaging

- [Wikipedia — Triple modular redundancy](https://en.wikipedia.org/wiki/Triple_modular_redundancy)
- [Wikipedia — Watchdog timer](https://en.wikipedia.org/wiki/Watchdog_timer)
- [Wikipedia — Circular buffer](https://en.wikipedia.org/wiki/Circular_buffer) (ring queue; lock-free SPSC planned)

## Planned (not in repo yet)

- Sensor threads + lock-free SPSC queues
- Dedicated watchdog thread
- GTest, CI, config generator, HMI
