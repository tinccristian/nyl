#pragma once

#include <string>

#include "core.h"

namespace nyl
{
    /**
     * @brief Thin wrapper over miniaudio's high-level engine.
     *
     * Init() once at startup, Shutdown() at exit. Play() is fire-and-forget for
     * one-shot SFX. Safe to call when uninitialised (no-ops) so headless runs
     * without an audio device don't crash.
     */
    class NYL_API AudioEngine
    {
    public:
        static bool Init();
        static void Shutdown();
        static bool IsInitialized();

        static void Play(const std::string& filepath);   // one-shot
        static void SetMasterVolume(float volume);
    };
}
