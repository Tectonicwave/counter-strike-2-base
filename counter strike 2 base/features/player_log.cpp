#include "player_log.h"

lag_record_t::lag_record_t(sdk::C_CSPlayerPawn* player)
{
	origin = player->get_game_scene_node()->get_vec_origin();
	abs_origin = player->get_game_scene_node()->get_vec_abs_origin();
	rotation = player->get_game_scene_node()->get_angle_rotation();
	abs_rotation = player->get_game_scene_node()->get_angle_rotation();
	simulation_time = player->get_simulation_time();
	memcpy(bone_array, player->get_game_scene_node()->get_skeleton_instance()->get_model_state().bones, sizeof(sdk::CBoneData) * 128);

}

void lag_record_t::Restore(sdk::C_CSPlayerPawn* player) {
	// Restore the pawn
	player->get_game_scene_node()->get_vec_origin() = origin;
	player->get_game_scene_node()->get_vec_abs_origin() = abs_origin;
	player->get_game_scene_node()->get_angle_rotation() = rotation;
	player->get_game_scene_node()->get_angle_rotation() = abs_rotation;
	player->get_simulation_time() = simulation_time;
	memcpy(player->get_game_scene_node()->get_skeleton_instance()->get_model_state().bones, bone_array, sizeof(sdk::CBoneData) * 128);
}

//this is public code
//for you lamos yes pasted 
float lag_record_t::calc_delta_time(int newest_tick) {
	int tick_count = manager->global_vars->tick_count;
	sdk::convar* sv_maxunlag = manager->cvar->find(utils::HashConst("sv_maxunlag"));
	float max_unlag = sv_maxunlag ? sv_maxunlag->value.fl : 0.2f;

	// Calculate modifiers and unlag ticks
	float unk_modifier = std::fmod(max_unlag, 0.015625f);
	float unlag_diff = max_unlag - unk_modifier;
	float time_modifier = unk_modifier * 64.0f;
	int unlag_ticks = static_cast<int>((unlag_diff * 64.0f) + 0.5f);

	// Initialize the lag compensation data for the newest tick
	lag_compensation_data_t newest_data;
	newest_data.m_tick = newest_tick;
	newest_data.m_time = newest_tick * 0.015625f;

	// Calculate the lower bound of ticks for lag compensation
	int lower_tick_bound = tick_count - unlag_ticks;
	float adjusted_time_modifier = (time_modifier > 0.0099999998f) ? -time_modifier : 0.0f;

	// Ensure time modifier wraps around correctly
	if (adjusted_time_modifier < 0.0f) {
		adjusted_time_modifier += 1.0f;  // Wrap around if negative
	}

	// Set the current data, adjusting the tick and time based on the time modifier
	lag_compensation_data_t current_data;
	current_data.m_time = adjusted_time_modifier;

	// Determine the tick based on the time modifier conditions
	if (time_modifier > (1.0f - 0.0099999998f)) {
		current_data.m_tick = lower_tick_bound - 1;
	}
	else {
		current_data.m_tick = lower_tick_bound;
	}

	if (adjusted_time_modifier > (1.0f - 0.0099999998f)) {
		current_data.m_tick += 1;
		current_data.m_time = 0.0f;
	}
	else if (adjusted_time_modifier <= 0.0099999998f) {
		current_data.m_time = 0.0f;
	}

	// Select the most relevant tick and data (either newest or current)
	int selected_tick = current_data.m_tick;
	lag_compensation_data_t selected_data = current_data;

	// Compare with newest data and select the most appropriate one
	if ((newest_data.m_tick + newest_data.m_time) * 0.015625f > 0.0f) {
		selected_tick = newest_data.m_tick;
		selected_data = newest_data;
	}

	// Calculate the tick difference and handle wraparound for time
	int tick_difference = selected_tick - current_data.m_tick;
	float time_difference = selected_data.m_time - current_data.m_time;

	// Handle negative time differences (wraparound)
	if (time_difference < 0.0f) {
		time_difference += 1.0f;  // Wraparound adjustment
	}

	int tick_delta = tick_difference - 1;
	if (time_difference >= 0.0f) {
		tick_delta = tick_difference;
	}

	// Final adjustment if time difference is near boundary
	if (time_difference >= (1.0f - 0.0099999998f)) {
		++tick_delta;
		time_difference = 0.0f;
	}
	else if (time_difference <= 0.0099999998f) {
		time_difference = 0.0f;
	}

	// Compute the final delta time
	return (tick_delta + time_difference) * 0.015625f;
}

bool lag_record_t::is_valid_time() {

	sdk::convar* sv_maxunlag = manager->cvar->find(utils::HashConst("sv_maxunlag"));
	float max_unlag = sv_maxunlag ? sv_maxunlag->value.fl : 0.2f;
	//float lag_compensation_factor = (manager->engine->get_net_channel_info()->get_network_latency() * 0.5f) + manager->engine->get_net_channel_info()->get_engine_latency();
	float server_time = (simulation_time * 64.0) + (manager->engine->get_net_channel_info()->get_network_latency() * 0.5f) + manager->engine->get_net_channel_info()->get_engine_latency();
	float delta_time = calc_delta_time(server_time);

	if (std::abs(delta_time) < max_unlag)
		return false;

	return true;
}

//FRAME_NET_UPDATE_END
void player_log::run_log(const sdk::client_frame_stage stage)
{

	/*
	if (!manager->engine->in_game())
	{
		// Clear all logs when not in-game
		for (auto i = 1; i <= manager->global_vars->max_clients; i++)
			logs[i].records.clear();
		return;
	}

	if (stage != sdk::client_frame_stage::FRAME_NET_UPDATE_END)
		return;

	if (!local_pawn)
		return;

	for (auto i = 1; i <= sdk::max_players; i++)
	{
		auto& cur_record = logs[i];
		const auto player_controller = manager->resource_service->game_entity_system->get_player_controller(i);

		if (!player_controller || !player_controller->find_class(FNV1A("CCSPlayerController")))
		{
			cur_record.records.clear();
			continue;
		}

		const auto player_pawn = player_controller->get_pawn();
		if (!player_pawn || !player_pawn->find_class(FNV1A("C_CSPlayerPawn")) || player_pawn->get_health() < 1.f)
		{
			cur_record.records.clear();
			continue;
		}

		// Handle respawn or new pawn
		if (cur_record.pawn != player_pawn)
		{
			cur_record.records.clear();
			cur_record.pawn = player_pawn;
			cur_record.controller = player_controller;
		}

		// Add new records if necessary
		if (cur_record.records.empty())
		{
			cur_record.records.emplace_back(player_pawn);
			continue;
		}

		lag_record_t record(player_pawn);
		cur_record.records.push_front(std::move(record));

		// Limit record size
		if (cur_record.records.size() > 32)
			cur_record.records.pop_back();

		// Remove invalid records
		if (cur_record.records.size() > 1 && !cur_record.records.front().is_valid_time())
		{
			cur_record.records.pop_front();
		}
	}*/
}
