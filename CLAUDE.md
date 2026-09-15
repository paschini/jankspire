# Jankspire

A from-scratch MMO learning project. Not aiming for a published game — the goal is
to find out what it actually takes to build an engine, and what that teaches about
which existing engine (if any) fits how I think. Full background lives in the
Obsidian note `Projekter/Jankspire/Project definition.md`; this file is the
condensed version a session needs without re-briefing.

## Philosophy

- **Deliberately naive first pass.** Lock in simple choices now, earn upgrades
  later once something actually runs. Don't design for problems that haven't
  shown up yet.
- **No engine** (Unity/Unreal/Godot/Bevy). Hand-building the engine and client
  is the point of this project, not a means to some other end.
- **Minimal libraries, but not the wild west.** Reach for the standard library
  first. Only add a new third-party dependency when the standard library
  genuinely can't do it, and call it out explicitly rather than quietly
  vendoring something in.

## Milestone 1 scope

- **Client**: C++ app renders a skinned FBX character, plays its walk cycle
  animation, and moves it around a scene with WASD input.
- **Server**: C# console app hosts multiple clients over the local network.
  Each client sees the others' positions update in (near) real time. Tested
  locally with 2–4 client instances.

## Repo structure

```
jankspire/
├── engine/               # C++ static lib — rendering, math, skeletal anim, ECS-ish core
├── client/               # C++ executable, links engine
├── server/               # standalone .NET console app
├── protocol/             # shared packet format spec (docs, not shared code)
│   └── packets.md
├── assets/
│   └── models/
├── third_party/          # Assimp etc.
└── CMakeLists.txt        # root: add_subdirectory(engine), add_subdirectory(client)
```

## Stack (locked for now)

- **Graphics**: Metal (Mac-only, and that's fine for this project). Not OpenGL
  (deprecated on macOS, capped at 4.1) or Vulkan (too much boilerplate for
  this stage).
- **Math**: Apple's `simd` framework — matrices, quaternions, dot/cross
  products. No external math library; integrates directly with Metal buffers.
- **Model import**: Assimp, loading FBX.
- **Networking**: raw sockets (`UdpClient` / BSD sockets), no networking
  framework, no shared codegen between client and server. Hand-designed
  plaintext packet format, documented in `protocol/packets.md` as it's built —
  don't let the two sides silently drift out of sync with the doc.
- **Server runtime**: C# / .NET console app.

## C++ coding standards (engine, client)

Target **C++20** — concepts, ranges, `span`, designated initializers are all
fair game; mature enough on current Xcode/clang not to fight the toolchain.

### Naming

Chosen to match the C# server's natural conventions, so reading both codebases
solo doesn't require a style context-switch.

| Kind | Convention | Example |
|---|---|---|
| Types (class/struct/enum/using) | `PascalCase` | `class Renderer`, `enum class RenderPass` |
| Functions & methods | `PascalCase` | `void Update()`, `Vector3 GetPosition()` |
| Member variables | `m_camelCase` | `m_position`, `m_meshBuffer` |
| Locals & parameters | `camelCase` | `deltaTime`, `vertexCount` |
| Constants / `constexpr` | `PascalCase` | `constexpr int MaxPlayers = 64;` |
| Namespaces | lowercase, short | `jankspire::render`, `jankspire::net` |
| Booleans | `Is`/`Has`/`Should` prefix | `bool IsAlive() const;`, `m_isVisible` |
| Files | `PascalCase` matching the primary type | `Renderer.h` / `Renderer.cpp` |

No Hungarian notation, no Unreal-style `U`/`A`/`F`/`b`/`T` prefixes — that
ceremony earns its keep in a huge multi-team codebase, not a solo hand-rolled
engine.

### Formatting

- Allman braces (opening brace on its own line) — again, matches the C#
  server's default so the eye doesn't have to switch modes.
- 4-space indents, no tabs.
- `#pragma once` for header guards, not include-guard macros.
- Local includes in quotes, system/third-party in angle brackets.

### Ownership & memory

- RAII everywhere. If it owns a resource (a Metal object, an Assimp scene, a
  socket), its lifetime is tied to a scope or an owning object — never manual
  alloc/free pairs.
- `std::unique_ptr` for single ownership. Avoid `std::shared_ptr` unless
  there's a genuine shared-ownership case; don't reach for it by default.
- Raw pointers and references mean "I don't own this, I'm just looking."
- No custom allocators or memory arenas yet — that's an optimization to earn
  once something is measurably slow, not a day-one decision.

### Error handling

- Exceptions are fine at boundaries: asset loading, startup/init,
  unrecoverable failures. (Assimp throws on bad input — that's fine, it
  happens at load time, not per-frame.)
- Nothing in the per-frame hot path (render loop, update loop) throws or
  allocates unnecessarily. Recoverable per-frame conditions return `bool` /
  `std::optional` and get handled inline.
- `assert()` for programmer-error invariants (things that should never happen
  if the code is correct). Explicit checks + logging for expected runtime
  conditions (a malformed packet, a missing file).

### Containers & STL

- `std::` containers by default (`vector`, `unordered_map`, `array`). Don't
  hand-roll a container unless you've actually measured a specific STL
  container as the bottleneck.
- Prefer `std::span` over raw pointer+length pairs where C++20 makes it fit.

### Warnings

- Build with `-Wall -Wextra -Wpedantic`. Don't silence a warning without
  understanding why it fired first.

## C# server

- Idiomatic .NET conventions — `PascalCase` types & methods, `camelCase`
  locals/params. This is just standard C#, nothing project-specific to
  remember here.
- Keep it a plain console app: no DI container, no ASP.NET, no ORM. Raw
  `UdpClient`, a simple loop, plain classes for per-client state.

## Testing

No formal test framework for Milestone 1. Correctness means running the
client plus 2–4 connected instances and watching it behave. Revisit a real
testing strategy once there's enough surface area to justify one.

## Build

CMake, target-based (`target_link_libraries` / `target_include_directories`,
not global `include_directories`). Root `CMakeLists.txt` adds `engine/` and
`client/` as subdirectories. `server/` builds independently via its own
`.csproj` / `dotnet build`, not through CMake.
