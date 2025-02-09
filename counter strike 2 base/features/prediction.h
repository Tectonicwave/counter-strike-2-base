#pragma once
#include "../manager.h"
#include "../sdk/interfaces/usercmd.h"

struct movement_data {
	int32_t fall_velocity = 0;
	int32_t flags = 0;
	int32_t ground_entity = 0;
	sdk::vector origin{};
	sdk::vector absolute_origin{};

	void Capture() {
		//fall_velocity = local_pawn->get_fall_velocity();
		//flags = local_pawn->get_player_flags();
		//origin = local_pawn->get_game_scene_node()->get_vec_origin();
		//absolute_origin = local_pawn->get_game_scene_node()->get_vec_abs_origin();
		//ground_entity = local_pawn->get_ground_entity();
	}

	void Restore() {
		//local_pawn->get_player_flags() = flags;
		//local_pawn->get_fall_velocity() = fall_velocity;
		//local_pawn->get_ground_entity() = ground_entity;
		//local_pawn->get_game_scene_node()->get_vec_origin() = origin;
		//local_pawn->get_game_scene_node()->get_vec_abs_origin() = absolute_origin;
	}
};

struct tickbase_t
{
	int tick_base = 0;
	int tick_count = 0;
	float interval_per_sub_tick = 0.0f;
	float current_time = 0.0f;
	float current_time2 = 0.0f;
	float frame_time = 0.0f;
	float frame_time2 = 0.0f;
};

struct prediction_info
{
	void reset()
	{
		*this = {};
	}

	uint32_t flags = 0;
	uint32_t prev_flags = 0;
	uint32_t move_type = 0;
	int32_t sequence = 0;
	float last_shot = 0.0f;
	float view_offset = 0.0f;
	sdk::vector origin{};
	sdk::vector velocity{};
	sdk::vector obb_maxs{};
	float view_delta = 0.0f;
	float view_height = 0.0f;
	int delta_tick = 0;
	bool in_prediction = false;
	bool first_prediction = false;
	bool has_been_predicted = false;
	bool should_predict = false;
	float spread = 0.0f;
	float accuracy = 0.0f;
	tickbase_t tickbase{};
};

struct prediction_t {
	void Begin(sdk::ccsgo_input* input, sdk::cuser_cmd* cmd);
	void end(sdk::ccsgo_input* input, sdk::cuser_cmd* cmd);

	bool is_prediction_ready() const {
		return manager->global_vars && local_pawn && local_controller && local_controller->get_pawn_alive();
	}

	//std::array<prediction_info, 150> pred_info = {};
	prediction_info pred_info{};
	int last_sequence = 0;
};

inline std::unique_ptr<prediction_t> prediction = std::make_unique<prediction_t>();

