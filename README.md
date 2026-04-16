# Planet Project (UE5.5)

A solo-developed sandbox exploration game inspired by Astroneer and Outer Wilds.  
Focused on survival and building mechanics, with expolration gameplay and Co-op.
This repositary will show you game structure, devlog, and some soruce code of game and gampeplay sysytems.

📅 Development started: February 20, 2026  
⚙️ Engine: Unreal Engine 5.5  
🎯 Genre: Survival, Advanture, Strategy

---

## Last implemented milestones

- [April 14, 2026 - Redesign signal field strength](docs/devlog/2026-04.md#april-14-2026---redesign-signal-field-strength)
- [April 16, 2026 - Update signal field algorithm](docs/devlog/2026-04.md#april-16-2026---update-signal-field-algorithm)
- [March 16, 2026 - Voxel Planet Integration](docs/devlog/2026-03.md#march-16-2026---voxel-planet-integration)

---

## Systems Overview

- [GravitySystem](docs/systems/gravity-system.md)
- [HQSignalSystem](docs/systems/hq-signal-system.md)

---

## Full Devlog Archive

- [February 2026](docs/devlog/2026-02.md)
- [March 2026](docs/devlog/2026-03.md)
- [April 2026](docs/devlog/2026-04.md)

---

## Project structure

```text
Source
└── Game
      └── Objectes and systems realted to game itself (UI manager, Game Instance, Game Modes)
└── Gameplay
      └── Objectes and systems realted to gameplay mechanics and features (HQSignalSystem, BuildingSystem)
