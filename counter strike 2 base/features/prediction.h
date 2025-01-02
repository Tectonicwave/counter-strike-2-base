#pragma once
#include "../manager.h"
#include "../sdk/interfaces/usercmd.h"

struct prediction_t
{
	struct prediction_info
	{
		void reset()
		{
			*this = {};
		}

		uint32_t flags{}, prev_flags{}, move_type{};
		int32_t sequence{};
		float last_shot{}, view_offset{};
		sdk::vector origin{}, velocity{};
		sdk::vector obb_maxs{};
		float view_delta{};
		float view_height{};

		int delta_tick{};

		bool in_prediction = false;
		bool first_prediction = false;
		bool hasbeen_prodicted = false;
		bool should_predict = false;

		float spreed = 0.0f;
		float accruracy = 0.0f;

		struct tickbase_t
		{
			int tick_base = 0;
			int tick_count = 0;
			float interval_per_sub_tick = 0.0f;
			float current_time = 0.0f;
			float current_time2 = 0.0f;

			float frame_time = 0.0f;
			float frame_time2 = 0.0f;
		} tickbase = {};
	};

	//std::array<prediction_info, 150> pred_info = {};
	prediction_info pred_info;
	int last_sequence = 0;

	bool is_prediction_ready() const {
		return manager->global_vars && local_pawn && local_controller && local_controller->get_pawn_alive();
	}

	void Begin(sdk::ccsgo_input* input, sdk::cuser_cmd* cmd);
	void end(sdk::ccsgo_input* input, sdk::cuser_cmd* cmd);
};

inline prediction_t* prediction = new prediction_t;