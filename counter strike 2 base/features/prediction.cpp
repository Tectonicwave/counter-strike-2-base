#include "prediction.h"
#include "../sdk/interfaces/network_client.h"

void RunPrediction(sdk::network_game_client_t* pNetworkGameClient, int nPredictionReason)
{
	using func_t = void(__fastcall*)(sdk::network_game_client_t*, int);
	static func_t fn = (func_t)FIND_PATTERN("engine2.dll", "40 55 41 56 48 83 EC ? 80 B9 F8 00 00 00 ?");
	fn(pNetworkGameClient, nPredictionReason);
}

//basic bare bones prediction
void prediction_t::Begin(sdk::ccsgo_input* input, sdk::cuser_cmd* cmd)
{
	if (!is_prediction_ready())
		return;

    auto* movement_services = local_pawn->get_movement_services();
    auto* network_game_client = manager->network_client_service->get_network_game_client();
    if (!movement_services || !network_game_client)
        return;

    auto& entry = pred_info;
    entry.flags = local_pawn->get_player_flags();

    // Store current global variables
    entry.tickbase.interval_per_sub_tick = manager->global_vars->interval_per_tick;
    entry.tickbase.current_time = manager->global_vars->cur_time;
    entry.tickbase.current_time2 = manager->global_vars->flCurrentTime2;
    entry.tickbase.tick_count = manager->global_vars->tick_count;
    entry.tickbase.frame_time = manager->global_vars->frame_time;
    entry.tickbase.frame_time2 = manager->global_vars->frame_time2;
    entry.tickbase.tick_base = local_controller->get_tick_base();

    // Store prediction state
    entry.in_prediction = manager->prediction->InPrediction;
    entry.first_prediction = manager->prediction->bFirstPrediction;
    entry.has_been_predicted = cmd->bHasBeenPredicted;
    entry.should_predict = network_game_client->should_predict;

    // Set prediction state
    cmd->bHasBeenPredicted = false;
    network_game_client->should_predict = true;
    manager->prediction->bFirstPrediction = false;
    manager->prediction->InPrediction = true;

    movement_services->SetPredictionCommand(cmd);
    local_controller->get_current_command() = cmd;

    // Update prediction
    if (network_game_client->should_predict && network_game_client->delta_tick > 0 && network_game_client->SomePrediction_tick > 0)
        RunPrediction(network_game_client, sdk::client_command_tick);
}

void prediction_t::end(sdk::ccsgo_input* input, sdk::cuser_cmd* cmd)
{
	if (!is_prediction_ready())// || this->last_sequence == input->nSequenceNumber)
		return;

	auto* movement_services = local_pawn->get_movement_services();
	auto* network_game_client = manager->network_client_service->get_network_game_client();
	if (!movement_services || !network_game_client)
		return;

	auto& entry = pred_info;//[input->nSequenceNumber % 150];

	movement_services->ResetPredictionCommand();
	local_controller->get_current_command() = nullptr;

    // Restore global variables
    manager->global_vars->interval_per_tick = entry.tickbase.interval_per_sub_tick;
    manager->global_vars->cur_time = entry.tickbase.current_time;
    manager->global_vars->flCurrentTime2 = entry.tickbase.current_time2;
    manager->global_vars->tick_count = entry.tickbase.tick_count;
    manager->global_vars->frame_time = entry.tickbase.frame_time;
    manager->global_vars->frame_time2 = entry.tickbase.frame_time2;
    local_controller->get_tick_base() = entry.tickbase.tick_base;

    // Restore prediction state
    manager->prediction->bFirstPrediction = entry.first_prediction;
    manager->prediction->InPrediction = entry.in_prediction;
    cmd->bHasBeenPredicted = entry.has_been_predicted;
    network_game_client->should_predict = entry.should_predict;
}