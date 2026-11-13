# KENZFLX

An insert effect plugin for Akai MPC standalone hardware, by KENZ.

KENZFLX combines drive, tone shaping, chorus, delay, reverb, bit-crushing and
tempo-synced time effects in one stereo insert. Reverse plays the preceding
quarter-note slice backwards; Half-Time replays the selected 1/2, 1, 2, or 4
bar loop at half speed; Glitch rearranges and stutters beat subdivisions;
Repeat loops the last captured subdivision at tempo-synced rates. The MPC UI
organizes controls into COLOR, MOD, SPACE and TIME pages, and refreshes
their displayed values when a built-in program is selected.

The effect is an experimental DSP implementation; listen for artifacts and
save projects before using it on a live set.

The built-in library contains 100 programs grouped by category: INIT/HALF (12),
FLEX (22), LOFI (15), REV (16), SPCE (16) and FX (19). The old preset Reverse
amount is assigned to the matching Half-Time, Glitch, or Reverse control.

## Build

```
python tools/build.py plugins/KENZFLX
python tools/package.py plugins/KENZFLX
```

## Project layout

- `plugin.json` - manifest (name, manufacturer, uid). The `uid` must match the C++ `uniqueId`.
- `src/KENZFLX.cpp` - the plugin source (parameters and DSP).
- `src/KenzflxPresets.h` - categorized built-in programs.
- `ui.json` - which parameters appear on the MPC screen, in which order.
- `build/` - build output (the deployable `KENZ - VST - KENZFLX` folder lives here).
- `dist/` - zips produced by `package.py`.
