#include "movement.h"
#include "../sdk/maths/math.h"

void movement_t::bhop(sdk::cuser_cmd* cmd) {

	auto base_cmd = cmd->csgoUserCmd.pBaseCmd;
	if (!base_cmd)
		return;

	const auto move_type = local_pawn->get_move_type();
	const auto in_water = local_pawn->get_water_level() >= 2;

	// Check if the player can jump (not in water or special move types)
	const bool can_jump = !in_water && move_type != sdk::MOVETYPE_LADDER &&
		move_type != sdk::MOVETYPE_NOCLIP && move_type != sdk::MOVETYPE_OBSERVER;

	// If we can't jump or the jump key isn't pressed, skip processing
	if (!can_jump) {
		return;
	}

	if (cmd->nButtons.nValue & sdk::IN_JUMP && local_pawn->get_player_flags() & sdk::fl_onground)
		cmd->nButtons.nValue &= ~sdk::IN_JUMP;
}

void movement_t::movement_fix(sdk::cuser_cmd* cmd) {

	// Validate command and base command pointers
	auto base_cmd = cmd->csgoUserCmd.pBaseCmd;
	if (!base_cmd || !base_cmd->pViewAngles)
		return;

	// Get current and target movement vectors
	sdk::vector2d original_movement(base_cmd->flForwardMove, base_cmd->flSideMove);
	sdk::vector2d adjusted_movement = original_movement;

	// Calculate yaw delta
	const sdk::qangle& target_view_angles = base_cmd->pViewAngles->angValue;
	float yaw_delta = math::normalize_yaw(manager->input->get_view_angles().yaw - target_view_angles.yaw);
	float yaw_radians = DEG2RAD(yaw_delta);

	// Adjust movement for view angle
	adjusted_movement.x = original_movement.x * std::cos(yaw_radians) - original_movement.y * std::sin(yaw_radians);
	adjusted_movement.y = original_movement.x * std::sin(yaw_radians) + original_movement.y * std::cos(yaw_radians);

	// Normalize movement if needed
	float magnitude = std::max(std::fabsf(adjusted_movement.x), std::fabsf(adjusted_movement.y));
	if (magnitude > 1.0f) {
		adjusted_movement.x /= magnitude;
		adjusted_movement.y /= magnitude;
	}

	// Update movement data
	manager->input->flForwardMove = adjusted_movement.x;
	manager->input->flSideMove = -adjusted_movement.y;

	// Update base command movement
	base_cmd->SetBits(sdk::PROTOSLOT_6 | sdk::PROTOSLOT_7);
	base_cmd->flForwardMove = adjusted_movement.x;
	base_cmd->flSideMove = adjusted_movement.y;
}