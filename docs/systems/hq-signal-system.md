# HQ Signal System

A modular gameplay signal system built for a sandbox exploration project in Unreal Engine 5.5.

The system is designed around **signal sources**, **signal receivers**, and a centralized **world subsystem** that evaluates signal strength at a given location.  
Its purpose is to provide a reusable foundation for mechanics such as communication range, proximity-based detection, environmental scanning, and signal-driven gameplay feedback.

---

## Purpose

The goal of this system is to simulate a signal network where:
- signal sources emit signal in a configurable range
- receivers query the signal state at their current location
- signal strength changes depending on distance
- attenuation can be customized with a curve
- gameplay or UI can react to signal updates through Blueprint events

This makes the system flexible enough for:
- HQ communication systems
- radar / scanner mechanics
- beacon detection
- exploration feedback
- area-based interaction logic

---

## Design Goals

- **Modular** — signal emitters and receivers are independent components
- **Reusable** — can be attached to different gameplay actors
- **Data-driven** — attenuation behavior can be configured with curves
- **Blueprint-friendly** — receiver broadcasts updates via multicast delegate
- **Lightweight** — current implementation avoids per-frame ticking

---

## High-Level Architecture

## Selected Source Files
- [HQSignalSystem.cpp](../../Source/HQSignalSystem/HQSignalSystem.cpp)
- [HQSignalReceiverComponent.cpp](../../Source/HQSignalSystem/HQSignalReceiverComponent.cpp)

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
