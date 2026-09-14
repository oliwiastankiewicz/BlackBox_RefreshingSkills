# BlackBox

A local event-processing and simulation system.

This project is a way for me to refresh and improve my programming skills in C, C++, C#, Python and Typescript by using the same underlying problem with a different architectural emphasis in each language.

PHASE 1
C
│
└── Generate deterministic event streams
        │
        ▼
PHASE 2
C++
│
├── Consume event streams
├── Process events
├── Optimize processing
├── Add concurrency
└── Benchmark
        │
        ▼
PHASE 3
C#
│
├── Store results
├── Manage simulations
├── HTTP API
└── Async jobs
        │
        ├──────────────┐
        ▼              ▼
PHASE 4              PHASE 5
Python               TypeScript
│                    │
├── Analysis         ├── REST
├── Statistics       ├── WebSockets
├── Reports          ├── Visualization
└── Graphs           └── Replay

---

C: Simulation Engine

- entity model
- event generation
- simulation clock
- deterministic simulation
- event serialization
- logging

C++: Event Processing Engine

- event parsing
- event validation
- event filtering
- event aggregation
- state reconstruction
- generic data structures
- concurrency
- benchmarking

C#: Application Layer

- HTTP API
- persistence
- simulation management
- async processing
- dependency injection
- automated tests

Phyton: Analytics

- event parsing
- statistics
- anomaly detection
- performance analysis
- visualization
- automated reports

TypeScript: Interface

- REST client
- WebSockets
- live visualization
- event filtering
- simulation timeline
- replay controls

## Version 1: Event-Driven Simulation Engine in C

Goal: Build a command-line event-driven simulation engine in pure C.

The engine will simulate a small artificial system containing devices that generate events. The engine processes those events in chronological order, updates the state of the simulated world, and records what happened.

The system contains entities such as:

- temperature sensors
- doors
- alarms
- machines
- jobs

These entities generate events. For example:

00:00:01 SENSOR_READING sensor=temperature-01 value=21.4\
00:00:03 DOOR_OPENED door=front\
00:00:04 ALARM_TRIGGERED alarm=main\
00:00:08 DOOR_CLOSED door=front\
00:00:09 ALARM_CLEARED alarm=main

The program must process these events in chronological order and maintain the state of the simulated system.

### Intended project structure

blackbox_c/\
│\
├── Makefile\
│\
├── include/\
│ ├── entity_base.h\
│ ├── entity.h\
│ ├── event.h\
│ ├── logger.h\
│ ├── queue.h\
│ ├── scenario.h\
│ ├── simulation.h\
│ └── timestamp.h\
│\
├── src/\
│ ├── entity.c\
│ ├── event.c\
│ ├── logger.c\
│ ├── main.c\
│ ├── queue.c\
│ ├── scenario.c\
│ ├── simulation.c\
│ └── timestamp.c\
│\
├── tests/\
│ ├── test_entity.c\
│ ├── test_event.c\
│ ├── test_logger.c\
│ ├── test_queue.c\
│ ├── test_scenario.c\
│ ├── test_simulation.c\
│ └── test_timestamp.c\
│\
├── scenarios/\
│ └── basic.txt\
│\
└── recordings/

### Core requirements

The program loads a scenario file describing:

- entities
- initial state
- events
- simulation parameters

The program maintains an event queue. The queue must keep events ordered by timestamp. Each event should contain at least:

- timestamp
- event type
- entity ID
- event-specific data

The simulation engine repeatedly:

- takes the next event
- advances simulation time
- processes the event
- updates system state
- records the result
- continues until no events remain

A basic simulation should produce readable output:

Black Box Simulation

Loaded scenario: warehouse.txt
Entities: 7
Events: 42

Starting simulation...

[000001] SENSOR temperature-01 = 21.4\
[000003] DOOR front OPENED\
[000004] ALARM main TRIGGERED\
[000008] DOOR front CLOSED\
[000009] ALARM main CLEARED

Simulation complete.

Processed events: 42
Simulation time: 00:12:43
