# Gravity System

A custom gravity system designed for spherical worlds in Unreal Engine 5.5.

This system enables gameplay on planets gravity. It using the Chaos Callbacks idea from [this article](https://dev.epicgames.com/community/learning/tutorials/lydy/unreal-engine-using-chaos-callbacks-for-a-custom-gravity-system-working-with-round-worlds) but implemented in diferent resuable classes. 

---

## Selected Source Files

- [PlanetGravitySystem.cpp](../../code/PlanetGravitySystem/PlanetGravitySystem.cpp)
- [GravitySourceComponent.cpp](../../code/PlanetGravitySystem/PlanetGravityAttractorComponent.cpp)
- [GravityMovementComponent.cpp](../../code/PLanetGravitySystem/PlanetGravityCharacterMovement.cpp)

---

## High-Level Architecture

```text
Planet Actor
└── Planet Gravity Attractor Component
      └── Defines gravity origin and strength
      └── Registers in gravity system

Physics / Gameplay Actor
└── Affected by gravity system
      └── Receives force toward nearest attractor

Character
└── Planet Gravity Character Movement
      └── Aligns orientation to gravity direction
      └── Handles movement on curved surfaces

PlanetGravitySystem (via Chaos callbacks)
└── Evaluates gravity per physics body
└── Applies forces toward attractors
└── Supports multiple gravity sources
