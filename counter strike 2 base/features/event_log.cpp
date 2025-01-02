#include "event_log.h"
#include <windows.h>
#include <mmsystem.h>  // This header is for PlaySoundA
#include <resources/hitsound.h>

const std::array<event_log::SoundData, 2> event_log::hit_sounds = {
	event_log::SoundData{ nullptr, 0 },            // Hitsound_None
	event_log::SoundData{ bell, sizeof(bell) },     // Hitsound_Custom2
};

void event_log::on_player_hurt(sdk::CGameEvent* event)
{
    if (!local_pawn || !event)
        return;

    auto helper = event->GetEventHelper();

    auto damage = helper.GetDamage();
    auto hitgroup = event->GetInt2("hitgroup", false);

    auto player = helper.GetPlayerController();
    auto attacker = helper.GetAttackerController();

    if (!player || !attacker)
        return;

    if (!attacker->get_is_local_player_controller())
        return;

    if (player->get_is_local_player_controller())
        return;

    // Use the correct index here to access the first sound
    const SoundData& sound_data = hit_sounds[1];
    if (sound_data.data && sound_data.size > 0) {
        // Play the sound from memory using SND_MEMORY
        PlaySoundA(reinterpret_cast<LPCSTR>(sound_data.data), NULL, SND_ASYNC | SND_MEMORY);
    }
}