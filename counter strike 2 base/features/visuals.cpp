#include "visuals.h"

#include "../resources/font_namespace.h"
#include "player_log.h"

//i like this methed more as im on ultra wide
bool visuals_t::get_player_rect(player_data_t& player)
{
	// Define top and bottom positions
	sdk::vector pos_top = player.pos + sdk::vector{ 0, 0, player.player_pawn->get_vec_view_offset().z + 8 };
	sdk::vector pos_bot = player.pos;
	pos_bot.z -= 4;

	// Convert world positions to screen positions
	if (!math::world_to_screen(pos_top, player.top) || !math::world_to_screen(pos_bot, player.bot))
		return false;

	// Round the screen coordinates
	player.top = player.top.round();
	player.bot = player.bot.round();

	// Calculate bounding box dimensions
	const auto& display_size = ImGui::GetIO().DisplaySize;
	player.height = round(clamp(player.bot.y - player.top.y, 10.f, clamp(display_size.y * 1.5f, 10.f, 999999.f)));
	player.width = round(clamp(player.height / 3.8f, 3.f, display_size.x / 4.f));

	// Adjust the top position based on calculated height
	player.top = player.bot;
	player.top.y -= player.height;

	// Check if the player is off-screen
	if (player.bot.x + player.width + 20 < 0 || player.bot.x - player.width - 20 > display_size.x || player.bot.y + 20 < 0 || player.bot.y - player.height - 20 > display_size.y)
		player.oof = true;

	return true;
}

void visuals_t::run()
{
	if (!manager->resource_service->game_entity_system || !manager->engine->in_game() || !local_pawn)
		return;

	players_get();
}

void visuals_t::render(const sdk::cview_setup* view_setup)
{
	if (!manager->resource_service->game_entity_system || !manager->engine->in_game() || !local_pawn)
		return;

	players_esp(view_setup);

}

void visuals_t::players_get()
{
	for (auto i = 1; i <= XOR_16(sdk::max_players); i++)
	{
		auto& player = players[i];
		const auto player_controller = manager->resource_service->game_entity_system->get_player_controller(i);
		if (!player_controller || !player_controller->find_class(FNV1A("CCSPlayerController")))
		{
			Reset_info(player, true);
			continue;
		}

		const auto player_pawn = player_controller->get_pawn();
		if (!player_pawn || !player_pawn->find_class(FNV1A("C_CSPlayerPawn")) || player_pawn->get_health() <= 0)
		{
			Reset_info(player, true);
			continue;
		}

		if (player_pawn == local_pawn)
		{
			Reset_info(player, true);
			continue;
		}

		if (player_pawn->get_last_spawn_time() != players[i].spawntime)
		{
			player.spawntime = player_pawn->get_last_spawn_time();
			player.alpha = 0.f;
			player.alpha_lerp = 0.f;
		}

		// lets store some of our values
		player.player_pawn = player_pawn;
		player.player_controller = player_controller;
		player.index = i;

		//we storing our info for players
		Store_info(player);

		if (!get_player_rect(player))
			player.oof = true;
	}
}

void visuals_t::players_esp(const sdk::cview_setup* view_setup)
{
	for (auto& player : players)
	{
		if (!player.valid)
			continue;

		player.offset = {};

		if (player.alpha > 0.f)
		{
			if (player.oof)
			{
				renderer->rect({ 50,50 }, { 100,100 }, { 255,255,255, 255 });
				continue;
			}

			//really should just add this etc into color
			const auto white = sdk::color{ 1.f, 1.f, 1.f, 1.f };  // Inside color

			Draw_box(player);

			Draw_Health(player);

			//yeah its bad lol
			auto& current_record = player_records->logs[player.index];

			if (current_record.pawn == player.player_pawn) {
				drawPlayerSkeleton(current_record.records.front().bone_array, white);

				drawPlayerSkeleton(current_record.records.back().bone_array, white);
			}

			// need set your font for text atm there here for basic reasons
			auto text_size = ImGui::CalcTextSize(player.player_controller->get_sanitized_player_name().get());
			sdk::vector2d textPosition = sdk::vector2d{ player.top.x, player.top.y - text_size.y - 2 };
			renderer->render_text(player.player_controller->get_sanitized_player_name().get(), textPosition, white, CENTERED_X, font::inter_element);
		}
	}
}

void visuals_t::Reset_info(player_data_t& player, bool force)
{
	player.alpha -= 255.f / 0.3f * manager->global_vars->frame_time;
	player.alpha = clamp(player.alpha, 0.f, 255.f);
	player.model_alpha -= 255.f / 0.3f * manager->global_vars->frame_time;
	player.model_alpha = clamp(player.model_alpha, 0.f, 255.f);
	player.health_interpolated = 0.f;

	// Check for disablers or force reset
	if (force)
	{
		player.alpha = 0.f;
		player.model_alpha = 0.f;
	}

	player.valid = false;
	player.handle = 0;

	// Update alpha_lerp
	player.alpha_lerp = player.alpha;
}

void visuals_t::drawPlayerSkeleton(const sdk::CBoneData* bones, sdk::color color) noexcept
{
	if (!bones)
		return;

	for (const auto& joints : BoneJointList::List)
	{
		sdk::CBoneData previous{};

		for (const auto index : joints)
		{
			const auto& current = bones[index];

			if (previous.location != sdk::vector(0, 0, 0))
			{
				sdk::vector2d start{}, end{};
				if (math::world_to_screen(previous.location, start) &&
					math::world_to_screen(current.location, end))
				{
					renderer->render_line(start, end, color, 1.f);
				}
			}

			previous = current;
		}
	}
}

void visuals_t::Store_info(player_data_t& player)
{
	player.valid = true;
	player.oof = false;
	player.pos = player.player_pawn->get_computed_abs_origin();
	player.model_alpha += 255.f / 0.5f * manager->global_vars->frame_time;
	player.model_alpha = clamp(player.model_alpha, 0.f, 255.f);
	player.alpha_lerp += 255.f / 0.6f * manager->global_vars->frame_time;
	player.alpha_lerp = clamp(player.alpha_lerp, 0.f, 255.f);
	player.alpha = 255.f;
	player.flash_duration = player.player_pawn->get_flash_duration();
}

void visuals_t::Draw_box(player_data_t& player)
{
	const auto black = sdk::color{ 0.f, 0.f, 0.f, 0.5f };  // Outline color
	const auto white = sdk::color{ 1.f, 1.f, 1.f, 1.f };  // Inside color

	// Define the initial rectangle using rect_legacy
	rect_legacy box(sdk::vector2d{ player.top.x - player.width, player.top.y }, player.width * 2, static_cast<float>(player.height));

	// Draw the first box (white, inside box)
	renderer->rect_outline(box.adjust_for_padding(1.f, 1.f).vec_min, box.adjust_for_padding(1.f, 1.f).vec_max, white);

	// Draw the second box (outer, black outline)
	renderer->rect_outline(box.vec_min, box.vec_max, black);

	// Draw the third box (black outline with additional padding)
	renderer->rect_outline(box.adjust_for_padding(2.f, 2.f).vec_min, box.adjust_for_padding(2.f, 2.f).vec_max, black);
}

// fluff trying do multiple bars when we can just use a function 
void visuals_t::draw_bar(const sdk::vector2d& start_pos, int length, bool horizontal, const std::vector<sdk::color>& colors, float fraction, std::optional<int> value)
{
	const auto white = sdk::color{ 1.f, 1.f, 1.f, 1.f };  // Inside color
	// Clamp the fraction to [0, 1]
	const float multiplier = std::clamp(fraction, 0.f, 1.f);
	const bool multicolor = colors.size() > 1;

	// Outline dimensions (4px thick black outline)
	const sdk::vector2d size = horizontal ? sdk::vector2d{ static_cast<float>(length), 4.f } : sdk::vector2d{ 4.f, -static_cast<float>(length) };

	// Create outline rectangle
	rect_legacy outline_rect(start_pos, size.x, size.y);

	// Draw the black outline
	renderer->rect_filled(outline_rect.vec_min, outline_rect.vec_max, sdk::color(0.f, 0.f, 0.f, 0.4f));

	// Bar dimensions (2px thick health bar)
	const float bar_length = std::floor((static_cast<float>(length) - 2.f) * multiplier);
	const sdk::vector2d bar_size = horizontal ? sdk::vector2d{ bar_length, 2.f } : sdk::vector2d{ 2.f, -bar_length };
	const sdk::vector2d bar_pos = horizontal ? start_pos + sdk::vector2d{ 1.f, 1.f } : start_pos + sdk::vector2d{ 1.f, -1.f };

	// Create bar rectangle
	rect_legacy bar_rect(bar_pos, bar_size.x, bar_size.y);

	// Determine the bar color (either gradient or single color)
	sdk::color bar_color = multicolor ? colors[0].lerp(colors[1], multiplier) : colors[0];

	// Draw the health bar
	renderer->rect_filled(bar_rect.vec_min, bar_rect.vec_max, bar_color);

	if (value.has_value())
	{
		//should change font lol
		sdk::vector2d text_pos = sdk::vector2d{
			horizontal ? start_pos.x + 2 + bar_length : start_pos.x,
			horizontal ? start_pos.y + 4 : start_pos.y - bar_length - 2
		};

		// Pass text_pos as a const reference to the render_text function
		renderer->render_text(std::to_string(value.value()), text_pos, white, DROP_SHADOW | RIGHT_ALIGN, font::inter_element);
	}
}

void visuals_t::Draw_Health(player_data_t& player)
{
	const auto health = player.player_pawn->get_health();  // Access the player's current health
	const auto max_health = std::max(health, player.player_pawn->get_max_health());  // Get max health

	// Define color range for health bar (green for high, red for low)
	const sdk::color full_hp_color{ 0.69f, 1.f, 0.31f, 1.f };  // Full health color (green)
	const sdk::color low_hp_color{ 1.f, 0.2f, 0.31f, 1.f };    // Low health color (red)

	// Calculate health fraction (0 to 1)
	float health_fraction = std::clamp(static_cast<float>(health) / max_health, 0.f, 1.f);

	// Position the health bar
	const auto start_pos = sdk::vector2d{ player.bot.x - player.width - 5, player.bot.y };

	// Draw the health bar
	draw_bar(start_pos, static_cast<int>(player.height), false, { low_hp_color, full_hp_color }, health_fraction, health);
}