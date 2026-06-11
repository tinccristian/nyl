// miniaudio implementation lives here (exactly one TU). Trim features we don't
// need to keep the build lean.
#define MA_NO_ENCODING
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "audio.h"
#include "log.h"

namespace nyl
{
    static ma_engine s_engine;
    static bool      s_initialized = false;

    bool AudioEngine::Init()
    {
        if (s_initialized) return true;
        if (ma_engine_init(NULL, &s_engine) != MA_SUCCESS)
        {
            NYL_CORE_ERROR("AudioEngine: failed to initialize (no audio device?)");
            return false;
        }
        s_initialized = true;
        NYL_CORE_INFO("AudioEngine initialized");
        return true;
    }

    void AudioEngine::Shutdown()
    {
        if (!s_initialized) return;
        ma_engine_uninit(&s_engine);
        s_initialized = false;
    }

    bool AudioEngine::IsInitialized() { return s_initialized; }

    void AudioEngine::Play(const std::string& filepath)
    {
        if (!s_initialized) return;
        if (ma_engine_play_sound(&s_engine, filepath.c_str(), NULL) != MA_SUCCESS)
            NYL_CORE_ERROR("AudioEngine: could not play '{}'", filepath);
    }

    void AudioEngine::SetMasterVolume(float volume)
    {
        if (s_initialized) ma_engine_set_volume(&s_engine, volume);
    }
}
