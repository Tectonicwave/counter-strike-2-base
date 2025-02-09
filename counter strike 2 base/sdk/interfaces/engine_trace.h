#pragma once
#include "../maths/vector3.h"
#include "../maths/vector.h"
#include "../game/entity_classes.h"
//alot of this is public

namespace sdk
{
	struct Ray_t
	{
	public:
		sdk::vector vec_start;
		sdk::vector vec_end;
		sdk::vector vec_mins;
		sdk::vector vec_maxs;
		Memory_pad(0x4);
		std::uint8_t unk_type;
	};

	struct SurfaceData_t
	{
	public:
		Memory_pad(0x8);
		float penetration_modifier;
		float damage_modifier;
		Memory_pad(0x4);
		int material;
	};

	struct TraceHitboxData_t
	{
	public:
		Memory_pad(0x38);
		int hit_group;
		Memory_pad(0x4);
		int hitbox_id;
	};

	//struct cs2_player_pawn;
	struct game_trace_t
	{
	public:
		game_trace_t() = default;

		void* surface;
		sdk::C_CSPlayerPawn* hit_entity;
		TraceHitboxData_t* hitbox_data;
		Memory_pad(0x38);
		std::uint32_t contents;
		Memory_pad(0x24);
		sdk::vector vec_start_pos;
		sdk::vector vec_end_pos;
		sdk::vector vec_normal;
		sdk::vector vec_position;
		Memory_pad(0x4);
		float fraction;
		Memory_pad(0x6);
		bool all_solid;
		Memory_pad(0x4D);
		SurfaceData_t* get_surface_data();
		int get_hitbox_id() const;
		int get_hitgroup() const;
		bool is_visible() const;
		bool did_hit_world() const;
	}; // Size: 0x108

	struct trace_filter_t
	{
	public:
		Memory_pad(0x8);
		std::int64_t trace_mask;
		std::array<std::int64_t, 2> v1;
		std::array<std::int32_t, 4> arr_skip_handles;
		std::array<std::int16_t, 2> arr_collisions;
		std::int16_t v2;
		std::uint8_t v3;
		std::uint8_t v4;
		std::uint8_t v5;

		trace_filter_t() = default;
		trace_filter_t(std::uint64_t uMask, sdk::C_CSPlayerPawn* pSkip1, sdk::C_CSPlayerPawn* pSkip2, int nLayer);
	};

	struct UpdateValue_t {
		float previous_length_mod{};
		float current_length_mod{};
		Memory_pad(0x8);
		std::int16_t handle_idx{};
		Memory_pad(0x6);
	};

	struct TraceArrElement_t {
		Memory_pad(0x30);
	};

	struct TraceData_t {
		std::int32_t uk1{};
		float uk2{ 52.0f };
		void* arr_pointer{};
		std::int32_t uk3{ 128 };
		std::int32_t uk4{ static_cast<std::int32_t>(0x80000000) };
		std::array< TraceArrElement_t, 0x80 > arr = {};
		Memory_pad(0x8);
		std::int64_t num_update{};
		void* pointer_update_value{};
		Memory_pad(0xC8);
		sdk::vector start{}, end{};
		Memory_pad(0x50);
	};


	class engine_trace_t
	{
	public:
		bool trace_shape(Ray_t* ray, sdk::vector start, sdk::vector end, trace_filter_t* filter, game_trace_t* game_trace)
		{
			using fn_trace_shape = bool(__fastcall*)(engine_trace_t*, Ray_t*, sdk::vector*, sdk::vector*, trace_filter_t*, game_trace_t*);
			static fn_trace_shape o_trace_shape = reinterpret_cast<fn_trace_shape>(
				utils::get_absolute_address(
					FIND_PATTERN("client.dll", "E8 ? ? ? ? 80 7D ? ? 75 ? F3 0F 10 05"), 0x1, 0x0));//"48 89 5C 24 20 48 89 4C 24 08 55 56 41 <-- direct signature

			return o_trace_shape(this, ray, &start, &end, filter, game_trace);
		}

		bool clip_ray_to_entity(Ray_t* ray, sdk::vector start, sdk::vector end, sdk::C_CSPlayerPawn* pawn, trace_filter_t* filter, game_trace_t* game_trace)
		{
			using fn_clip_ray_to_entity = bool(__fastcall*)(engine_trace_t*, Ray_t*, sdk::vector*, sdk::vector*, sdk::C_CSPlayerPawn*, trace_filter_t*, game_trace_t*);
			static fn_clip_ray_to_entity o_clip_ray_to_entity = reinterpret_cast<fn_clip_ray_to_entity>(
				FIND_PATTERN("client.dll", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 00 00 00 48 8B 9C")
				);

			return o_clip_ray_to_entity(this, ray, &start, &end, pawn, filter, game_trace);
		}

		void initialize_trace(game_trace_t& trace)
		{
			using function_t = void(__fastcall*)(game_trace_t&);
			static function_t fn = reinterpret_cast<function_t>(
				FIND_PATTERN("client.dll", "48 89 5C 24 08 57 48 83 EC 20 48 8B D9 33 FF 48 8B 0D")
				);

			fn(trace);
		}

		void clip_trace_to_players(vector3& start, vector3& end, trace_filter_t* filter, game_trace_t* trace, float min, int length, float max)
		{
			using function_t = void(__fastcall*)(vector3&, vector3&, trace_filter_t*, game_trace_t*, float, int, float);
			static function_t fn = reinterpret_cast<function_t>(
				FIND_PATTERN("client.dll", "48 8B C4 55 56 48 8D A8 48 FF FF FF 48 81 EC A8 01 00 00 48 89 58 08 49")
				);

			fn(start, end, filter, trace, min, max, length);
		}

		bool clip_to_ent(Ray_t* ray, vector3 start, vector3 end, sdk::C_CSPlayerPawn* pawn, trace_filter_t* filter, game_trace_t* game_trace)
		{
			using fn_clip_ray_entity = bool(__fastcall*)(engine_trace_t*, Ray_t*, vector3*, vector3*, sdk::C_CSPlayerPawn*, trace_filter_t*, game_trace_t*);
			static fn_clip_ray_entity o_clip_ray_entity = reinterpret_cast<fn_clip_ray_entity>(
				FIND_PATTERN("client.dll", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0")
				);

			return o_clip_ray_entity(this, ray, &start, &end, pawn, filter, game_trace);
		}

		static void get_trace_info(TraceData_t* trace, game_trace_t* hit, const float unknown_float, void* unknown)
		{
			using function_t = void(__fastcall*)(TraceData_t*, game_trace_t*, float, void*);
			static function_t fn = reinterpret_cast<function_t>(
				FIND_PATTERN("client.dll", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 60 48 8B E9 0F")
				);

			fn(trace, hit, unknown_float, unknown);
		}

		static void crate_trace(TraceData_t* const trace, const vector3 start, const vector3 end, trace_filter_t& filler, const int penetration_count)
		{
			using function_t = void(__fastcall*)(TraceData_t*, vector3, vector3, trace_filter_t, int);
			static function_t fn = reinterpret_cast<function_t>(
				FIND_PATTERN("client.dll", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 83 EC 40 F2")
				);

			fn(trace, start, end, filler, penetration_count);
		}
	};

}