# Planet Project (UE5.5)

A solo-developed sandbox exploration game inspired by Astroneer and Outer Wilds.  
Focused on survival and building mechanics, with expolration gameplay and Co-op.
This repositary will show you game structure, devlog, and some soruce code of game and gampeplay sysytems.

📅 Development started: February 20, 2026  
⚙️ Engine: Unreal Engine 5.5  
🎯 Genre: Survival, Advanture, Strategy

---

## Last implemented milestones

- [April 14, 2026 — HQ Signal Field Update](docs/devlog/2026-04.md#april-14-2026--redesign-signal-field-strength)
- [March 16, 2026 — Voxel Planet Integration](docs/devlog/2026-03.md#march-16-2026--voxel-planet-integration)
- [March 15, 2026 — Character Gravity Movement](docs/devlog/2026-03.md#march-15-2026--character-gravity-movement)

---

## Systems Overview

- [GravitySystem](docs/systems/gravity-system.md)
- [HQSignalSystem](docs/systems/hq-signal-system.md)

---

## Full Devlog Archive

- [February 2026](docs/devlog/2026-02.md)
- [March 2026](docs/devlog/2026-03.md)

---

## Project structure

```text
Source
└── Game
      └── Objectes and systems realted to game itself (UI manager, Game Instance, Game Modes)
└── Gameplay
      └── Objectes and systems realted to gameplay mechanics and features (HQSignalSystem, BuildingSystem)
