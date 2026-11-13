#pragma once

struct KenzflxPreset
{
    const char* name;
    float drive;
    float warmth;
    float tone;
    float chorus;
    float delay;
    float reverb;
    float crush;
    float mix;
    float reverse;
};

inline constexpr KenzflxPreset kKenzflxPresets[] = {
    // INIT / HALF
    {"[INIT] Clean Default",   0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f},
    {"[HALF] Pure 0.5x",       0.00f, 0.00f, 0.45f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.20f},
    {"[HALF] 50% Sub Layer",   0.12f, 0.35f, 0.38f, 0.00f, 0.00f, 0.10f, 0.00f, 0.50f, 0.20f},
    {"[HALF] Trap Melody",     0.18f, 0.50f, 0.34f, 0.25f, 0.22f, 0.35f, 0.00f, 0.85f, 0.22f},
    {"[HALF] Dark Drill",      0.26f, 0.62f, 0.26f, 0.20f, 0.28f, 0.42f, 0.14f, 0.90f, 0.22f},
    {"[HALF] Chopped Screwed", 0.22f, 0.70f, 0.28f, 0.35f, 0.15f, 0.25f, 0.16f, 1.00f, 0.24f},
    {"[HALF] Lo-Fi Tape",      0.24f, 0.75f, 0.30f, 0.30f, 0.12f, 0.20f, 0.26f, 0.90f, 0.20f},
    {"[HALF] Vinyl Slowdown",  0.16f, 0.65f, 0.35f, 0.40f, 0.00f, 0.18f, 0.18f, 0.85f, 0.18f},
    {"[HALF] 808 Sub Boost",   0.35f, 0.60f, 0.20f, 0.00f, 0.00f, 0.05f, 0.10f, 0.45f, 0.20f},
    {"[HALF] Dreamy Pad",      0.08f, 0.48f, 0.40f, 0.55f, 0.32f, 0.60f, 0.00f, 0.80f, 0.24f},
    {"[HALF] Ghostly Slow",    0.14f, 0.55f, 0.32f, 0.30f, 0.35f, 0.65f, 0.08f, 0.75f, 0.22f},
    {"[HALF] Slow Echoes",     0.12f, 0.40f, 0.42f, 0.22f, 0.48f, 0.38f, 0.00f, 0.70f, 0.20f},

    // FLEX
    {"[FLEX] Shuffle 1/8",        0.10f, 0.30f, 0.50f, 0.15f, 0.12f, 0.20f, 0.00f, 1.00f, 0.30f},
    {"[FLEX] Chop 1/16",          0.14f, 0.35f, 0.52f, 0.20f, 0.18f, 0.22f, 0.00f, 1.00f, 0.32f},
    {"[FLEX] Melody Flipper",     0.12f, 0.42f, 0.48f, 0.30f, 0.24f, 0.32f, 0.00f, 0.90f, 0.35f},
    {"[FLEX] Beat Re-Arrange",    0.15f, 0.38f, 0.50f, 0.18f, 0.15f, 0.25f, 0.05f, 1.00f, 0.38f},
    {"[FLEX] Syncopated Chop",    0.18f, 0.45f, 0.46f, 0.22f, 0.26f, 0.28f, 0.08f, 0.95f, 0.40f},
    {"[FLEX] Trap Stutter 4th",   0.20f, 0.40f, 0.50f, 0.15f, 0.20f, 0.24f, 0.00f, 1.00f, 0.42f},
    {"[FLEX] Triplet Rework",     0.12f, 0.36f, 0.52f, 0.25f, 0.22f, 0.30f, 0.00f, 0.95f, 0.45f},
    {"[FLEX] Drill Counter",      0.22f, 0.55f, 0.34f, 0.28f, 0.28f, 0.42f, 0.12f, 0.85f, 0.48f},
    {"[FLEX] Reverse Chop 1/4",   0.14f, 0.44f, 0.46f, 0.32f, 0.25f, 0.36f, 0.00f, 0.90f, 0.50f},
    {"[FLEX] Ping-Pong Slice",    0.10f, 0.32f, 0.54f, 0.35f, 0.40f, 0.30f, 0.00f, 0.85f, 0.52f},
    {"[FLEX] Vinyl Brake Bar",    0.18f, 0.58f, 0.40f, 0.20f, 0.15f, 0.25f, 0.14f, 1.00f, 0.54f},
    {"[FLEX] Tape Stop 1/8",      0.20f, 0.52f, 0.44f, 0.15f, 0.18f, 0.22f, 0.10f, 1.00f, 0.56f},
    {"[FLEX] Bounce Gate 16th",   0.16f, 0.38f, 0.52f, 0.24f, 0.28f, 0.26f, 0.06f, 0.95f, 0.58f},
    {"[FLEX] Off-Beat Melody",    0.12f, 0.40f, 0.48f, 0.30f, 0.32f, 0.35f, 0.00f, 0.90f, 0.60f},
    {"[FLEX] Glitch Arp Slice",   0.28f, 0.30f, 0.56f, 0.25f, 0.30f, 0.28f, 0.34f, 0.90f, 0.62f},
    {"[FLEX] Half-Rev Hybrid",    0.18f, 0.54f, 0.36f, 0.34f, 0.26f, 0.40f, 0.10f, 0.95f, 0.64f},
    {"[FLEX] DJ Scratch Loop",    0.25f, 0.45f, 0.48f, 0.10f, 0.14f, 0.18f, 0.22f, 1.00f, 0.66f},
    {"[FLEX] Stutter Cascade",    0.15f, 0.35f, 0.54f, 0.28f, 0.38f, 0.34f, 0.05f, 0.85f, 0.68f},
    {"[FLEX] Afro Rhythm Flip",   0.12f, 0.42f, 0.52f, 0.36f, 0.30f, 0.28f, 0.00f, 0.80f, 0.70f},
    {"[FLEX] UKG Skip Chop",      0.16f, 0.36f, 0.54f, 0.22f, 0.24f, 0.24f, 0.08f, 0.95f, 0.72f},
    {"[FLEX] Broken Record",      0.26f, 0.62f, 0.36f, 0.28f, 0.20f, 0.26f, 0.28f, 0.95f, 0.74f},
    {"[FLEX] BoomBap Chop 94",    0.24f, 0.56f, 0.42f, 0.12f, 0.14f, 0.20f, 0.32f, 0.95f, 0.34f},

    // LOFI
    {"[LOFI] VHS 1989",           0.22f, 0.75f, 0.32f, 0.25f, 0.10f, 0.15f, 0.18f, 0.85f, 0.00f},
    {"[LOFI] Dusty MPC60",        0.20f, 0.45f, 0.44f, 0.00f, 0.00f, 0.08f, 0.32f, 0.90f, 0.00f},
    {"[LOFI] SP-1200 Grit",       0.35f, 0.50f, 0.56f, 0.00f, 0.00f, 0.05f, 0.38f, 0.95f, 0.00f},
    {"[LOFI] Warped Vinyl",       0.15f, 0.65f, 0.38f, 0.35f, 0.00f, 0.18f, 0.12f, 0.80f, 0.00f},
    {"[LOFI] Broken Walkman",     0.30f, 0.85f, 0.28f, 0.45f, 0.00f, 0.10f, 0.25f, 0.90f, 0.00f},
    {"[LOFI] Phone Booth",        0.55f, 0.20f, 0.72f, 0.00f, 0.00f, 0.05f, 0.45f, 1.00f, 0.00f},
    {"[LOFI] Sunbaked Tape",      0.25f, 0.80f, 0.34f, 0.30f, 0.12f, 0.20f, 0.15f, 0.85f, 0.00f},
    {"[LOFI] Underwater",         0.10f, 0.70f, 0.15f, 0.45f, 0.20f, 0.45f, 0.05f, 0.90f, 0.00f},
    {"[LOFI] Old Mellotron",      0.18f, 0.65f, 0.36f, 0.28f, 0.15f, 0.35f, 0.28f, 0.85f, 0.00f},
    {"[LOFI] Late Night Jazz",    0.12f, 0.58f, 0.40f, 0.20f, 0.12f, 0.25f, 0.08f, 0.75f, 0.00f},
    {"[LOFI] Melted Wax",         0.20f, 0.78f, 0.26f, 0.50f, 0.18f, 0.22f, 0.14f, 0.85f, 0.00f},
    {"[LOFI] 8-Bit Boy",          0.15f, 0.10f, 0.60f, 0.00f, 0.00f, 0.00f, 0.78f, 0.85f, 0.00f},
    {"[LOFI] Dirty Diode",        0.62f, 0.45f, 0.42f, 0.00f, 0.00f, 0.10f, 0.15f, 0.75f, 0.00f},
    {"[LOFI] Reel To Reel",       0.20f, 0.52f, 0.47f, 0.08f, 0.06f, 0.12f, 0.05f, 0.70f, 0.00f},
    {"[LOFI] Crushed Soul",       0.32f, 0.68f, 0.38f, 0.18f, 0.14f, 0.28f, 0.42f, 0.85f, 0.00f},

    // REV
    {"[REV] Full Reverse",        0.00f, 0.20f, 0.50f, 0.00f, 0.00f, 0.15f, 0.00f, 1.00f, 1.00f},
    {"[REV] Reverse Swell",       0.10f, 0.35f, 0.45f, 0.25f, 0.25f, 0.45f, 0.00f, 0.75f, 0.88f},
    {"[REV] Backwards Tape",      0.25f, 0.70f, 0.35f, 0.20f, 0.15f, 0.25f, 0.18f, 0.90f, 1.00f},
    {"[REV] Ghost Echoes",        0.05f, 0.30f, 0.40f, 0.30f, 0.45f, 0.50f, 0.00f, 0.70f, 0.84f},
    {"[REV] Shimmer Flip",        0.05f, 0.25f, 0.65f, 0.45f, 0.35f, 0.60f, 0.00f, 0.75f, 0.50f},
    {"[REV] Bit Reverse",         0.30f, 0.15f, 0.58f, 0.10f, 0.25f, 0.15f, 0.68f, 0.85f, 0.80f},
    {"[REV] Horror Tape",         0.40f, 0.75f, 0.20f, 0.40f, 0.30f, 0.65f, 0.25f, 0.95f, 0.95f},
    {"[REV] Vocal Flip",          0.15f, 0.35f, 0.54f, 0.25f, 0.32f, 0.35f, 0.00f, 0.60f, 0.50f},
    {"[REV] Guitar Rewind",       0.18f, 0.48f, 0.46f, 0.38f, 0.26f, 0.38f, 0.06f, 0.70f, 0.70f},
    {"[REV] Synth Backspin",      0.24f, 0.40f, 0.52f, 0.42f, 0.34f, 0.42f, 0.12f, 0.80f, 0.85f},
    {"[REV] Broken Loop",         0.35f, 0.55f, 0.36f, 0.30f, 0.40f, 0.25f, 0.48f, 0.85f, 0.65f},
    {"[REV] Cloud Reverse",       0.00f, 0.35f, 0.42f, 0.55f, 0.38f, 0.70f, 0.00f, 0.75f, 0.55f},
    {"[REV] Crunch Flip",         0.50f, 0.45f, 0.48f, 0.10f, 0.20f, 0.18f, 0.52f, 0.80f, 0.75f},
    {"[REV] Haunted Hall",        0.15f, 0.50f, 0.30f, 0.30f, 0.35f, 0.75f, 0.10f, 0.85f, 0.60f},
    {"[REV] Digital Tear",        0.45f, 0.10f, 0.64f, 0.15f, 0.30f, 0.15f, 0.75f, 0.80f, 0.70f},
    {"[REV] Time Fracture",       0.32f, 0.40f, 0.50f, 0.35f, 0.52f, 0.45f, 0.38f, 0.85f, 0.80f},

    // SPCE
    {"[SPCE] Juno Chorus",        0.08f, 0.35f, 0.52f, 0.65f, 0.00f, 0.18f, 0.00f, 0.75f, 0.00f},
    {"[SPCE] 80s Detune",         0.10f, 0.40f, 0.54f, 0.78f, 0.15f, 0.28f, 0.00f, 0.80f, 0.00f},
    {"[SPCE] Slapback",           0.15f, 0.30f, 0.48f, 0.00f, 0.35f, 0.10f, 0.00f, 0.65f, 0.00f},
    {"[SPCE] Dub Tape Echo",      0.30f, 0.65f, 0.34f, 0.15f, 0.62f, 0.30f, 0.14f, 0.75f, 0.00f},
    {"[SPCE] Ping-Pong Wide",     0.05f, 0.25f, 0.55f, 0.25f, 0.55f, 0.25f, 0.00f, 0.70f, 0.00f},
    {"[SPCE] Infinite Wash",      0.05f, 0.40f, 0.42f, 0.45f, 0.40f, 0.85f, 0.00f, 0.80f, 0.00f},
    {"[SPCE] Small Room",         0.08f, 0.30f, 0.48f, 0.05f, 0.00f, 0.28f, 0.00f, 0.55f, 0.00f},
    {"[SPCE] Dark Cavern",        0.18f, 0.60f, 0.24f, 0.30f, 0.35f, 0.78f, 0.08f, 0.80f, 0.00f},
    {"[SPCE] Shimmer Sky",        0.05f, 0.20f, 0.68f, 0.55f, 0.35f, 0.75f, 0.00f, 0.70f, 0.00f},
    {"[SPCE] Lush Pad Maker",     0.10f, 0.50f, 0.44f, 0.68f, 0.30f, 0.65f, 0.00f, 0.80f, 0.00f},
    {"[SPCE] Dotted 1/8 Vibe",    0.08f, 0.30f, 0.54f, 0.20f, 0.46f, 0.24f, 0.00f, 0.60f, 0.00f},
    {"[SPCE] Frozen Cloud",       0.05f, 0.45f, 0.40f, 0.72f, 0.45f, 0.90f, 0.00f, 0.85f, 0.00f},
    {"[SPCE] Studio Air",         0.04f, 0.20f, 0.56f, 0.12f, 0.08f, 0.22f, 0.00f, 0.40f, 0.00f},
    {"[SPCE] Deep Cathedral",     0.08f, 0.42f, 0.45f, 0.32f, 0.28f, 0.82f, 0.00f, 0.75f, 0.00f},
    {"[SPCE] Chorus Echoes",      0.12f, 0.45f, 0.46f, 0.58f, 0.44f, 0.35f, 0.00f, 0.70f, 0.00f},
    {"[SPCE] Spring Surf",        0.20f, 0.40f, 0.56f, 0.15f, 0.12f, 0.45f, 0.05f, 0.65f, 0.00f},

    // FX
    {"[FX] Phonk Cowbell",        0.65f, 0.40f, 0.58f, 0.45f, 0.18f, 0.28f, 0.22f, 0.85f, 0.00f},
    {"[FX] Afro Guitar",          0.12f, 0.42f, 0.52f, 0.38f, 0.32f, 0.26f, 0.00f, 0.65f, 0.00f},
    {"[FX] Techno Rumble",        0.58f, 0.70f, 0.18f, 0.10f, 0.35f, 0.45f, 0.20f, 0.75f, 0.00f},
    {"[FX] Synthwave 84",         0.22f, 0.48f, 0.54f, 0.62f, 0.28f, 0.40f, 0.05f, 0.75f, 0.00f},
    {"[FX] Trap Bell Shine",      0.15f, 0.30f, 0.60f, 0.35f, 0.36f, 0.44f, 0.00f, 0.65f, 0.25f},
    {"[FX] Industrial Crush",     0.78f, 0.35f, 0.46f, 0.15f, 0.20f, 0.30f, 0.72f, 0.85f, 0.00f},
    {"[FX] 808 Saturator",        0.48f, 0.65f, 0.36f, 0.00f, 0.00f, 0.05f, 0.14f, 0.65f, 0.00f},
    {"[FX] Chillhop Rhodes",      0.16f, 0.62f, 0.40f, 0.40f, 0.18f, 0.28f, 0.12f, 0.75f, 0.00f},
    {"[FX] Dark Trap Sample",     0.26f, 0.66f, 0.30f, 0.30f, 0.26f, 0.48f, 0.18f, 0.80f, 0.30f},
    {"[FX] Fuzz Guitar",          0.82f, 0.50f, 0.44f, 0.20f, 0.15f, 0.25f, 0.28f, 0.85f, 0.00f},
    {"[FX] Dream Pop",            0.14f, 0.46f, 0.48f, 0.60f, 0.38f, 0.62f, 0.04f, 0.75f, 0.15f},
    {"[FX] Crunchy Drums",        0.42f, 0.52f, 0.48f, 0.05f, 0.05f, 0.14f, 0.36f, 0.60f, 0.00f},
    {"[FX] Cyber Bit",            0.38f, 0.20f, 0.62f, 0.45f, 0.34f, 0.30f, 0.66f, 0.80f, 0.00f},
    {"[FX] Soul Sample Flip",     0.20f, 0.60f, 0.42f, 0.22f, 0.16f, 0.24f, 0.16f, 0.75f, 0.20f},
    {"[FX] Reggaeton Pluck",      0.16f, 0.38f, 0.54f, 0.30f, 0.34f, 0.28f, 0.00f, 0.60f, 0.00f},
    {"[FX] Neo Soul Warmth",      0.18f, 0.64f, 0.42f, 0.44f, 0.14f, 0.26f, 0.08f, 0.70f, 0.00f},
    {"[FX] Grime Bass Grit",      0.60f, 0.55f, 0.40f, 0.15f, 0.10f, 0.15f, 0.44f, 0.75f, 0.00f},
    {"[FX] Hard Clip Bus",        0.70f, 0.45f, 0.50f, 0.00f, 0.00f, 0.08f, 0.18f, 0.55f, 0.00f},
    {"[FX] Cosmic Dust",          0.15f, 0.50f, 0.46f, 0.55f, 0.46f, 0.64f, 0.24f, 0.75f, 0.35f},
};

inline constexpr int kNumPresets =
    static_cast<int>(sizeof(kKenzflxPresets) / sizeof(kKenzflxPresets[0]));
