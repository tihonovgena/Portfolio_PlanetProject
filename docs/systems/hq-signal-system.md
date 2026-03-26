# HQ Signal System

A gameplay-driven signal system designed for a sandbox exploration project in Unreal Engine 5.5.

This system is not just a technical feature — it is a **core gameplay mechanic** that defines how the player interacts with the world.

The player is represented as a central "HQ" (base) that can deploy and control robots.  
However, control over a robot is only possible while it remains within signal coverage.

This creates a gameplay loop where **exploration is constrained and extended through signal infrastructure**.

---

## Gameplay Concept

- The player acts as a central base (HQ) that makes decisions
- The base can deploy robots to interact with the world
- Each robot can only be controlled while within signal range
- If signal is lost:
  - the robot becomes inactive
  - control returns to the base
  - the player must deploy a new robot

To explore further, the player must **extend signal coverage** using:
- temporary relay objects (similar to tethers)
- permanent structures (antennas, towers)
- future systems (e.g. satellites)

This makes signal not just a UI value, but a **core progression and exploration mechanic**.

---

## Purpose

The system provides a foundation for:
- limiting player reach through signal coverage
- creating exploration paths via signal extension
- enabling risk/reward decisions (go further vs lose connection)
- supporting future large-scale communication systems

---

## Design Goals

- **Gameplay-first** — system directly supports core player loop
- **Modular** — signal emitters and receivers are independent components
- **Reusable** — applicable to different gameplay actors and systems
- **Data-driven** — attenuation controlled via curves
- **Blueprint-friendly** — signal updates exposed via events
- **Lightweight** — avoids per-frame ticking

---

## High-Level Architecture

## Selected Source Files
- [HQSignalSystem.cpp](../../code/HQSignalSystem/HQSignalSystem.cpp)
- [HQSignalReceiverComponent.cpp](../../code/HQSignalSystem/HQSignalReceiverComponent.cpp)

## Detailed System Breakdown
- [HQ Signal System — Full Technical Breakdown](../systems/hq-signal-system.md)

```text
Signal Source Actor
└── UHQSignalSourceComponent
      └── Registers itself in UHQSignalSystem

Signal Receiver Actor
└── UHQSignalReceiverComponent
      └── Periodically requests signal data from UHQSignalSystem
      └── Broadcasts OnSignalUpdated(SignalData)

UHQSignalSystem (UWorldSubsystem)
└── Stores registered signal sources
└── Evaluates strongest signal for a world location
└── Returns FHQSignalData
