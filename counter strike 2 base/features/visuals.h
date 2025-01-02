#pragma once
#include "../manager.h"
#include "../sdk/interfaces/usercmd.h"

#include "../../sdk/maths/math.h"
#include "../../renderer/renderer.h"
#include <draw_manager.h>
#include "../renderer/rect_legacy.h"

struct visuals_t
{
	struct player_data_t
	{
		sdk::vector pos{};
		sdk::C_CSPlayerPawn* player_pawn = nullptr;
		sdk::CCSPlayerController* player_controller = nullptr;

		float x;
		float y;
		float w;
		float h;

		sdk::vector2d top{};
		sdk::vector2d bot{};

		struct
		{
			bool alive{};
			sdk::vector pos{};
			sdk::qangle viewangles{};
		}local{};

		struct
		{
			int top;
			int right;
			int bottom;
			int left;
			int text_left;
			int text_right;
		} offset{};

		float height{};
		float width{};
		float fade_time{};
		float alpha{};
		float alpha_lerp{};
		float model_alpha{};
		float spawntime{};
		float health_interpolated{};
		float ping{};
		float flash_duration{};

		int armor{};
		int health{};

		int index{};
		int handle = 0;

		bool valid{};
		bool scoped{};
		bool oof{};
	};

	struct BoneJointList
	{
		static inline const std::vector<DWORD> Trunk = { sdk::neck_0, sdk::spine_2, sdk::pelvis };
		static inline const std::vector<DWORD> LeftArm = { sdk::neck_0, sdk::arm_upper_L, sdk::arm_lower_L, sdk::hand_L };
		static inline const std::vector<DWORD> RightArm = { sdk::neck_0, sdk::arm_upper_R, sdk::arm_lower_R, sdk::hand_R };
		static inline const std::vector<DWORD> LeftLeg = { sdk::pelvis, sdk::leg_upper_L, sdk::leg_lower_L, sdk::ankle_L };
		static inline const std::vector<DWORD> RightLeg = { sdk::pelvis, sdk::leg_upper_R, sdk::leg_lower_R, sdk::ankle_R };
		static inline const std::vector<std::vector<DWORD>> List = { Trunk, LeftArm, RightArm, LeftLeg, RightLeg };
	};

	bool get_player_rect(player_data_t& player);

	void run();

	void render(const sdk::cview_setup* view_setup);

	void players_get();

	void players_esp(const sdk::cview_setup* view_setup);

	void Reset_info(player_data_t& player, bool force);

	void Store_info(player_data_t& player);

	void Draw_box(player_data_t& player);

	void draw_bar(const sdk::vector2d& start_pos, int length, bool horizontal, const std::vector<sdk::color>& colors, float fraction, std::optional<int> value);

	void Draw_Health(player_data_t& player);

	player_data_t players[sdk::max_players];
};

inline visuals_t* visuals = new visuals_t;