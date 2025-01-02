#include "events.h"
#include <string>

#include "../features/event_log.h"

sdk::events sdk::evnt{};

void sdk::events::fire_game_event(sdk::CGameEvent* event)
{
	//std::string name = event->GetName();
	switch (utils::fnv1a(event->GetName()))
	{
	case FNV1A("player_hurt"):
		eventlog->on_player_hurt(event);
		break;
	default:
		break;
	}
}