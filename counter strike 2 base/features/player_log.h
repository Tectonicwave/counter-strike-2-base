#pragma once
#include "../manager.h"
#include "../sdk/interfaces/usercmd.h"
#include <deque>

//this is basic lagcomp
struct lag_record_t {
	bool valid = true;
	sdk::CBoneData bone_array[128]{};
	float simulation_time = 0.f;
	sdk::C_BaseEntity* target = nullptr;
	sdk::vector origin{}, abs_origin{};
	sdk::qangle rotation{}, abs_rotation{};

	lag_record_t() = default;
	lag_record_t(sdk::C_CSPlayerPawn* player);

	void Restore(sdk::C_CSPlayerPawn* player);

	float calc_delta_time(int newest_tick);

	struct lag_compensation_data_t
	{
		std::uint64_t m_tick;
		float m_time;
		Memory_pad(0x24);
	};

	bool is_valid_time();
};


struct player_log_t
{
	void reset(sdk::C_CSPlayerPawn* player)
	{
		records.clear();
	}

	float m_spawntime = 0.f;

	sdk::CCSPlayerController* controller{ nullptr };
	sdk::C_CSPlayerPawn* pawn{ nullptr };
	std::deque<lag_record_t> records;
};

struct player_log {

	player_log_t logs[65] = {};

	std::array<player_log_t, 65> entries;

	void run_log(const sdk::client_frame_stage stage);

	player_log_t player_records[65];
};

inline std::unique_ptr<player_log> player_records = std::make_unique<player_log>();
