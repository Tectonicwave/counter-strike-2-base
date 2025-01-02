#pragma once

#include "game_event_manager.h"

namespace sdk
{
	class events : public sdk::game_event_listener
	{
	public:
		void fire_game_event(sdk::CGameEvent* event);
	};

	extern events evnt;
}