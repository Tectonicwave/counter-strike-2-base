#pragma once
#include "../manager.h"
#include "../sdk/interfaces/usercmd.h"
#include <deque>

//this is basic lagcomp
struct lag_record_t {
    bool valid = true;
    sdk::CBoneData bone_array[128]{}; // Bone data for the player
    float simulation_time = 0.f;      // Simulation time of the player
    sdk::C_BaseEntity* target = nullptr; // Target entity
    sdk::vector origin{}, abs_origin{};  // Origin and absolute origin
    sdk::qangle rotation{}, abs_rotation{}; // Rotation and absolute rotation

    lag_record_t() = default;
    lag_record_t(sdk::C_CSPlayerPawn* player); // Constructor to initialize with player data

    void Restore(sdk::C_CSPlayerPawn* player); // Restore player state from this record
    float calc_delta_time(int newest_tick);    // Calculate delta time for lag compensation
    bool is_valid_time();                      // Check if the record's time is valid

    struct lag_compensation_data_t {
        std::uint64_t m_tick; // Tick count
        float m_time;         // Time associated with the tick
        Memory_pad(0x24);     // Padding for alignment
    };
};


struct player_log_t {
    void reset(sdk::C_CSPlayerPawn* player) {
        records.clear(); // Clear all records for this player
    }

    float m_spawntime = 0.f; // Spawn time of the player
    sdk::CCSPlayerController* controller{ nullptr }; // Player controller
    sdk::C_CSPlayerPawn* pawn{ nullptr };            // Player pawn
    std::deque<lag_record_t> records;                // Queue of lag records
};

struct player_log {
    std::array<player_log_t, 65> logs;

    void run_log(const sdk::client_frame_stage stage); // Run the logging logic for each frame
};

inline std::unique_ptr<player_log> player_records = std::make_unique<player_log>();
