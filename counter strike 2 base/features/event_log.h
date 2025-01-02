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

inline event_log* eventlog = new event_log;