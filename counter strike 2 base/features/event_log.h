#pragma once
#include "../manager.h"

class event_log
{
public:

    struct SoundData {
        unsigned char* data;
        size_t size;
    };

    // Global array of hit sounds
    static const std::array<SoundData, 2> hit_sounds;

    void on_player_hurt(sdk::CGameEvent* event);

};

inline std::unique_ptr<event_log> eventlog = std::make_unique<event_log>();