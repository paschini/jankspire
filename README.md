# Jankspire

### ⚠️ Here be dragons.

This is a personal learning project, not production software. Expect naive
first-pass code, sharp edges, and decisions that will visibly get better (or
get thrown out) over time. If you've landed here from a resume or portfolio
link: yes, it's rough on purpose — this is a working log of learning to build
a game engine from scratch, not a polished product.

## What this is

A from-scratch MMO learning project. Not aiming for a published game — the
goal is to find out what it actually takes to build an engine, and what that
teaches about which existing engine (if any) fits how I think.

Deliberately naive first pass. Lock in simple choices now, earn upgrades
later once something actually runs. No Unity, Unreal, Godot, or Bevy — the
engine and client are hand-built from here.

## Milestone 1 scope

- **Client**: C++ app renders a skinned FBX character, plays its walk cycle
  animation, and moves it around a scene with WASD input.
- **Server**: C# console app hosts multiple clients over the local network.
  Each client sees the others' positions update in (near) real time. Tested
  locally with 2–4 client instances.

## Stack

| Layer | Choice | Why |
|---|---|---|
| Graphics | Metal | Mac-only, and that's fine here. OpenGL is deprecated on macOS; Vulkan is too much boilerplate for this stage. |
| Math | Apple's `simd` | Integrates directly with Metal buffers, no external math library needed. |
| Model import | Assimp (FBX) | Hand-parsing FBX's binary format is miserable; Assimp does it once at load time. |
| Networking | Raw sockets (`UdpClient` / BSD sockets) | Hand-designed plaintext protocol, no framework, no shared codegen — see [`protocol/packets.md`](protocol/packets.md). |
| Server runtime | C# / .NET | Console app, no frameworks. |

## Repo structure

```
jankspire/
├── engine/               # C++ static lib — rendering, math, skeletal anim, ECS-ish core
├── client/                # C++ executable, links engine
├── server/                # standalone .NET console app
├── protocol/              # shared packet format spec (docs, not shared code)
│   └── packets.md
├── assets/
│   └── models/
├── third_party/           # Assimp etc.
└── CMakeLists.txt         # root: add_subdirectory(engine), add_subdirectory(client)
```

## Status

Early scaffolding — folder structure and coding standards are in place;
no engine/client/server code has been written yet.

See [`CLAUDE.md`](CLAUDE.md) for the full coding standards and project
context (naming conventions, error handling, memory ownership rules, build
setup).

## Setup and running instructions:
Please refer to each poject's own instructions:

- Engine: [Running the editor](./engine/README.md)

