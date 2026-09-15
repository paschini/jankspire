# Packet format

Hand-designed plaintext protocol between client and server. Not shared code —
each side implements its own parsing, and this doc is the only contract
between them. Keep it in sync as packets get added or change; if the doc and
an implementation disagree, the doc is wrong and needs fixing too.

Nothing is locked yet — this will fill in as the client/server actually get
built.

## Early inspiration (non-binding)

Rough shape from a much older, ~30-year-ago first attempt at this. Not a
spec, just a starting shape:

- `WLK N33` — walk command, direction/position payload
- `ATK 023` — attack target in front, with weapon/target id
- `CHA ...` — chat message
- Server replies with position/state updates.
