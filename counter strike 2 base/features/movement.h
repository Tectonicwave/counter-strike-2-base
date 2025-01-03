#pragma once
#include "../manager.h"
#include "../sdk/interfaces/usercmd.h"

struct movement_t
{

	struct jump_t {
		bool onground_tick;
		bool remove_tick;
	};

	std::array<jump_t, 150> jump = {};

	void bhop(sdk::cuser_cmd* cmd);

	void movement_fix(sdk::cuser_cmd* cmd);

	void proccess_sub_tick_movement(sdk::cuser_cmd* cmd, int subtickIndex);

};

inline std::unique_ptr<movement_t> movement = std::make_unique<movement_t>();
