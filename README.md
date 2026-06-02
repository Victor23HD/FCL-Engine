# FCL-Engine

A concurrent and deterministic simulation engine in modern C++ focused on the management and degradation of Flight Control Laws (FCL) under critical hardware failures.

The objective of this project is to demonstrate high-reliability software infrastructure (DAL A compliance) and reactive fault-mitigation at runtime.

---

## 🛠️ Low-Level Strict Constraints

The engine is developed under strict constraints typical of safety-critical embedded systems:

* **Zero Dynamic Allocation:** The use of `new`, `malloc`, or dynamic containers is strictly forbidden during the active cycle. All memory is pre-allocated in static memory pools at startup to ensure predictability.
* **No Exception Handling (`try-catch`):** Purely deterministic execution flow. Errors are mapped and handled in constant time via fixed-size static types to eliminate undefined behavior.
* **Temporal Determinism:** No I/O operations, file parsing, or blocking mechanisms (locks) at runtime.

---

## 🎯 Implemented Critical Components

1. **Telemetry Layer:** Isolated threads communicating via lock-free circular buffers to eliminate race conditions.
2. **Triple Modular Redundancy (TMR):** Voting logic mechanism to isolate faulty sensors and consolidate valid telemetry data.
3. **Active Watchdog:** Strict millisecond-level heartbeat monitoring designed to trigger emergency state transitions (*Normal Law* ➔ *Alternate Law* ➔ *Direct Law*).
4. **Offline/Online Validation (DO-330):** An offline utility (`/generator`) validates engineering limits and generates immutable data structures (`constexpr`) at compile-time, shielding the runtime environment.

---

## 📂 Repository Structure

```text
├── .github/workflows/   # CI/CD Pipeline (Build & Test via GTest)
├── generator/           # Offline tool for parameter validation
├── include/fcl/         # Engine headers and fixed-size type definitions
├── src/                 # System threads, TMR, and FCL Engine implementation
└── tests/               # Automated unit tests (GoogleTest)
