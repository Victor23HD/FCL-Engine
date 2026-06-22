# FCL-Engine

C++20 simulator for flight control law degradation under sensor faults.

**MVP (motor core): 100%** — sensors, TMR, SPSC queues, watchdog thread, flight mode engine, unit tests.

**Full project: ~35%** — Docker, Raspberry Pi, config generator, HMI 2D and other portfolio phases are not started yet.

Not production avionics — learning/portfolio project.

## What runs today (MVP)

Build with CMake 3.20+:

```bash
cmake -B build -DFCL_BUILD_TESTS=ON
cmake --build build
./build/Debug/fcl_hello          # Windows: build\Debug\fcl_hello.exe
ctest --test-dir build -C Debug  # unit tests
```

### Demo cycles (`fcl_hello`)

**Cycle 1 — TMR pressure fault**

```
3 sensor threads -> SPSC queue per channel
TMR vote (sensor 3 @ 2500 bar faulty)
FlightControlEngine -> Alternate
```

**Cycle 2 — watchdog heartbeat fault**

```
sensor 2: freeze heartbeat (fault inject)
burst samples + watchdog thread polling
WatchdogHeartbeatLost -> Direct
```

## Layout

```text
include/fcl/     core, sensors, messaging, voting, watchdog, flight
src/             main, fcl_engine (sensor, voter, watchdog, flight law)
tests/           GTest
.github/         CI (build + test on push)
```

## MVP checklist (done)

| Item | Status |
|------|--------|
| ISensor + VirtualHydraulicSensor + fault inject | done |
| SPSC lock-free queue per channel | done |
| 1 sensor thread per channel | done |
| TMR 2-of-3 voter | done |
| Watchdog monitor thread | done |
| FlightControlEngine (Normal / Alternate / Direct) | done |
| SystemEvent propagation | done |
| GTest + CI | done |

**Not in MVP** (enum exists, logic not wired yet): `Emergency` mode, full event matrix (GearStuck, PitotSpeedLoss, …).

## Full project roadmap (not MVP)

| Phase | Target | Status |
|-------|--------|--------|
| 1 — Motor core (this repo today) | MVP | **100%** |
| 2 — Docker x86 reproducible build | dev container | not started |
| 3 — Raspberry Pi / cross-compile ARM | embedded target | not started |
| 4 — Offline config generator | DO-330-style validation | not started |
| 5 — HMI 2D + ECAM | WebSocket, separate process | not started |

## References

Material I read while shaping this code (links verified manually):

### Flight control & FBW

- [Skybrary — Flight Control Laws](https://skybrary.aero/articles/flight-control-laws)
- [Airbus — Safety innovation #1: Fly-by-wire (FBW)](https://www.airbus.com/en/newsroom/stories/2022-06-safety-innovation-1-fly-by-wire-fbw)

### Redundancy, watchdog, messaging

- [Wikipedia — Triple modular redundancy](https://en.wikipedia.org/wiki/Triple_modular_redundancy)
- [Wikipedia — Watchdog timer](https://en.wikipedia.org/wiki/Watchdog_timer)
- [Wikipedia — Circular buffer](https://en.wikipedia.org/wiki/Circular_buffer)
