# HQ Signal System

A gamplay features of game, where main player character and other vehicles, constraited to explore world by signal of base, 
it is not able to explore world without signal from the base.

The player is represented as a central "HQ" (base) that can deploy and control robots.  
However, control over a robot is only possible while it remains within signal coverage.

## Selected Source Files
- [HQSignalSystem.cpp](../../code/HQSignalSystem/HQSignalSystem.cpp)
- [HQSignalReceiverComponent.cpp](../../code/HQSignalSystem/HQSignalReceiverComponent.cpp)

## High-Level Architecture
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
