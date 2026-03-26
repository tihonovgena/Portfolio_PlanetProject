# Planetary Gravity System

A custom gravity system designed for spherical worlds in Unreal Engine 5.5.

This system is a **core world simulation layer** that enables gameplay on planets with non-linear gravity fields.

Instead of relying on default Z-axis gravity, the system allows actors to be affected by **dynamic gravity attractors**, making it possible to walk, move, and simulate physics on curved planetary surfaces.

---

## Gameplay Concept

- The world consists of spherical planets acting as gravity attractors
- Each planet pulls actors toward its center
- Actors align their orientation to the current gravity direction
- Multiple attractors can exist simultaneously

This allows:
- walking around full planets
- seamless movement across curved surfaces
- physically consistent interactions in planetary environments

---

## Purpose

The system provides a foundation for:
- planetary exploration gameplay
- non-linear movement (walking on spheres)
- physics interactions in curved worlds
- integration with character movement systems

It replaces the default global gravity with a **localized and dynamic gravity model**.

---

## Design Goals

- **System-driven** — gravity defined by world objects, not global constants
- **Modular** — gravity sources are independent components
- **Scalable** — supports multiple attractors
- **Physics-integrated** — works with Chaos Physics callbacks
- **Reusable** — applicable to both characters and physics actors

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
